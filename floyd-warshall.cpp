/*******************************************
Floyd-Warshall algorithm implementation.
*******************************************/

#include <iostream>
#include <vector>

using namespace std;

const int32_t INF = 1e9;

struct {
    void search(vector<vector<int>> & d) {
        // d is adjacent matrix
        int n = d.size();
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (d[i][k] < INF && d[k][j] < INF)
                        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }
    }
} FloydWarshallSearch;
