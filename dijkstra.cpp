/*******************************************
Dijkstra algorithm implementation.
*******************************************/

#include <iostream>
#include <vector>
#include <set>

using namespace std;

const int64_t INF = 9e18;

class Graph {
    int n;  // number of verticies
public:
    vector<vector<pair<int, int64_t>>> e;  // adjacency list
    Graph(int num_of_verticies) {
        n = num_of_verticies;
        e = vector<vector<pair<int, int64_t>>> (n, vector<pair<int, int64_t>>());
    }
    int size() {
        return n;
    }
    void add_edge(int from_vertex, int to_vertex, int64_t weight) {
        e[from_vertex].push_back({to_vertex, weight});
    }
    vector<pair<int, int64_t>> const& neighbours(int vertex) {
        return e[vertex];
    }
    void print_adj_list() {
        for (int i = 0; i < n; ++i) {
            cout << i << ": ";
            for (int j = 0; j < e[i].size(); ++j)
                cout << e[i][j].first << " ";
            cout << endl;
        }
    }
};

struct Dijkstra {
    Graph * G;
    int n;
    vector<int64_t> d;  // distance
    vector<int> p;      // parent

    void search(Graph * graph, int s) {
        G = graph;
        n = G->size();
        p.assign(n, -1);
        d.assign(n, INF);
        d[s] = 0;

        set<pair<int64_t, int>> q;
        q.insert({d[s], s});

        while (!q.empty()) {
            // extract min
            int u = q.begin()->second;
            q.erase(q.begin());
            // relax neighbours
            int v;
            int64_t w;
            for (auto to : G->neighbours(u)) {
                int v = to.first;
                int64_t w = to.second;
                if (d[v] > d[u] + w) {
                    q.erase({d[v], v});
                    d[v] = d[u] + w;
                    p[v] = u;
                    q.insert({d[v], v});
                }
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m, s;
    cin >> n >> m >> s;

    Graph graph = Graph(n);

    int u, v;
    int64_t w;
    for (int i = 0; i < m; ++i) {
        cin >> u >> v >> w;
        graph.add_edge(u-1, v-1, w);
    }

    Dijkstra djk;
    djk.search(&graph, s-1);

    for (int i = 0; i < n; ++i) {
        if (djk.d[i] != INF)
            cout << djk.d[i] << " ";
        else
            cout << "-1 ";
    }
}
