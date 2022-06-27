/*******************************************
BFS algorithm implementation.
*******************************************/

#include <iostream>
#include <vector>
#include <queue>

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

struct BFS {
    Graph * G;
    int n;             // |V(G)|
	vector<int> dist;  // distances from start vertex
	int start_v;       // start vertex

    void init(Graph * graph) {
        G = graph;
        n = graph->size();
        dist = vector<int>(n, -1);
    }

    void search(Graph * graph, int start_vertex) {
        init(graph);
        search(start_vertex);
    }

    void search(Graph * graph, vector<int> start_vertices) {
        init(graph);
        for (auto v : start_vertices) {
            if (dist[v] == -1)
                search(v);
        }
    }

    void search(int start_vertex) {
        start_v = start_vertex;
        queue<int> q;
        q.push(start_v);
        dist[start_v] = 0;
        int u;

        while(!q.empty()) {
            u = q.front();
            q.pop();
            for(auto & v : G->neigbours(u)) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
    }
};
