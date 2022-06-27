/*******************************************
DFS algorithm implementation with example
of searching strongly connected components.
*******************************************/

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

const int64_t INF = 9e18;

class Graph {
    int n;  // number of verticies
public:
    vector<vector<int>> e;  // adjacency list
    Graph(int num_of_verticies) {
        n = num_of_verticies;
        e = vector<vector<int>> (n, vector<int>());
    }
    int size() {
        return n;
    }
    void add_edge(int from_vertex, int to_vertex) {
        e[from_vertex].push_back(to_vertex);
    }
    vector<int> const& neigbours(int vertex) {
        return e[vertex];
    }
    vector<int> get_sinks() {
        vector<int> sinks;
        for (int v = 0; v < n; ++v) {
            if (e[v].size() == 0)
                sinks.push_back(v);
        }
        return sinks;
    }
    void print_adj_list() {
        for (int i = 0; i < n; ++i) {
            cout << i << ": ";
            for (int j = 0; j < e[i].size(); ++j)
                cout << e[i][j] << " ";
            cout << endl;
        }
    }
};

struct DFS {
    Graph * G;
    int n;          // |V(G)|
	vector<int> d;  // opening time (d[u] > 0 - vertex is opened)
	vector<int> f;  // closing time
	vector<int> cc; // connectivity components ids
	int t;          // current time
	int cur_cc;     // current connectivity component

	void init(Graph * graph) {
        G = graph;
        n = graph->size();
        d = vector<int>(n, 0);
        f = vector<int>(n, 0);
        cc = vector<int>(n, 0);
        t = 1;
        cur_cc = 0;
	}

    void search(Graph * graph) {
        init(graph);
        for (int v = 0; v < n; ++v) {
            if (d[v] == 0) {
                ++cur_cc;
                search(v);
            }
        }
    }

    void ordered_search(Graph * graph, vector<int> const& order) {
        init(graph);
        for (auto v : order) {
            if (d[v] == 0) {
                ++cur_cc;
                search(v);
            }
        }
    }

    void search(int v) {
        cc[v] = cur_cc;
        d[v] = t++;
        for (auto & u : G->neigbours(v)) {
            if (d[u] == 0)
                search(u);
        }
        f[v] = t++;
    }

    vector<int> get_top_sort() {
        vector<pair<int, int>> fv;
        for (int v = 0; v < n; ++v) {
            fv.push_back(make_pair(f[v], v));
        }
        sort(fv.rbegin(), fv.rend());

        vector<int> order(n);
        for (int i = 0; i < n; ++i) {
            order[i] = fv[i].second;
        }
        return order;
    }

    vector<int> get_cc_members() {
        vector<int> cc_members(cur_cc, 0);
        for (int v = 0; v < n; ++v) {
            if (cc_members[cc[v]-1] == 0)
                cc_members[cc[v]-1] = v;
        }
        return cc_members;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    Graph graph = Graph(n);
    Graph graph_tr = Graph(n);

    int a, b;
    for (int i = 0; i < m; ++i) {
        cin >> a >> b;
        // vertices enumerates from 0
        graph.add_edge(a-1, b-1);
        graph_tr.add_edge(b-1, a-1);
    }

    DFS dfs;
    // topological sort of graph
    dfs.search(&graph);
    vector<int> order = dfs.get_top_sort();
    // search strongly connected components using inverted graph
    dfs.ordered_search(&graph_tr, order);
    // creare set of condens edges
    set<pair<int, int>> condens_edges;
    for (int u = 0; u < n; ++u) {
        for (auto v : graph_tr.neigbours(u)) {
            if (dfs.cc[u] != dfs.cc[v])
                condens_edges.insert({dfs.cc[v], dfs.cc[u]});
        }
    }
    // assemble condens graph
    Graph condens_graph = Graph(dfs.cur_cc);
    for (auto [u, v] : condens_edges) {
        condens_graph.add_edge(u-1, v-1);
    }
    // search sink verticies (without outgoing edges)
    vector<int> condens_sinks = condens_graph.get_sinks();

    vector<int> cc_members = dfs.get_cc_members();
    cout << condens_sinks.size() << "\n";
    for (auto condens_sink : condens_sinks) {
        cout << cc_members[condens_sink] + 1 << " ";
    }
}
