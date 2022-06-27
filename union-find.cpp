/*******************************************
Union-Find data structure implementation.
*******************************************/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct UnionFind {
    int n;          // size
    vector<int> p;  // representative
    vector<int> r;  // rank

    UnionFind(int n) {
        p.assign(n, 0);
        r.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            p[i] = i;
        }
    }

    // with path compression
    int get(int x) {
        if (p[x] != x)
            p[x] = get(p[x]);
        return p[x];
    }

    // with rank heuristic
    void join(int x, int y) {
        x = get(x);
        y = get(y);
        if (x == y)
            return;
        if (r[x] > r[y])
            swap(x, y);
        p[x] = y;
        if (r[x] == r[y])
            ++r[y];
        return;
    }
};
