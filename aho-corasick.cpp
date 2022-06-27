/*******************************************
Aho-Corasick algorithm implementation.
*******************************************/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;
const int ALPHABET_SIZE = 'z' - 'a' + 1;

struct Node {
    vector<Node *> next;
    bool is_terminal;
    Node * parent;
    Node * suff_link;
    char inchar;
    vector<Node *> linked_from;
    int first_vis_iter;
    int vis_cnt;
    vector<int> pattern_num;
    int pattern_len;
    int index;

    Node(Node * p, char ch, int idx) {
        next.assign(ALPHABET_SIZE, nullptr);
        is_terminal = false;
        parent = p;
        inchar = ch;
        vis_cnt = 0;
        first_vis_iter = INF;
        suff_link = nullptr;
        index = idx;
    }
};

struct Trie {
    vector<Node *> nodes;
    int k = ALPHABET_SIZE;
    Node * state;
    int iter;
    vector<pair<int, pair<int,int>>> stats;

    Trie() {
        Node * root = new Node(nullptr, '.', 0);
        nodes.push_back(root);
        state = root;
        iter = 0;
    }

    void add(const string & s, int pattern_num) {
        Node * v = nodes[0];
        for (char ch : s) {
            int idx = ch - 'a';
            if (v->next[idx] == nullptr) {
                nodes.push_back(new Node(v, ch, nodes.size()));
                v->next[idx] = nodes.back();
            }
            v = v->next[idx];
        }
        v->is_terminal = true;
        v->pattern_num.push_back(pattern_num);
        v->pattern_len = s.size();
    }

    void create_suff_links() {
        queue<Node *> q;
        q.push(nodes[0]);
        while(!q.empty()) {
            Node * u = q.front();
            q.pop();
            for (auto v : u->next) {
                if (v != nullptr) q.push(v);
            }

            if (u->parent != nullptr) {
                int idx = u->inchar - 'a';
                Node * v = u->parent->suff_link;
                while (v != nullptr && v->next[idx] == nullptr) {
                    v = v->suff_link;
                }
                if (v != nullptr) {
                    u->suff_link = v->next[idx];
                    v->next[idx]->linked_from.push_back(u);
                }
                else {
                    u->suff_link = nodes[0];
                    nodes[0]->linked_from.push_back(u);
                }
            }
        }
    }

    void update_state(char ch) {
        int idx = ch - 'a';
        ++iter;

        while (state->next[idx] == nullptr && state->suff_link != nullptr)
            state = state->suff_link;

        if (state->next[idx] != nullptr) {
            state = state->next[idx];
            if (state->first_vis_iter == INF)
                state->first_vis_iter = iter;
            state->vis_cnt++;
        }
    }

    void update_stats(Node * u) {
        for (Node * v : u->linked_from) {
            update_stats(v);
            u->first_vis_iter = min(u->first_vis_iter, v->first_vis_iter);
            u->vis_cnt += v->vis_cnt;
        }
        if (u->is_terminal) {
            int u_first_vis_iter = u->first_vis_iter;
            if (u_first_vis_iter == INF) {
                u_first_vis_iter = 0;
            }
            else {
                u_first_vis_iter -= (u->pattern_len - 1);
            }

            pair<int, int> u_stat = make_pair(u_first_vis_iter, u->vis_cnt);
            for (auto pattern_num : u->pattern_num)
                stats.push_back(make_pair(pattern_num, u_stat));
        }
    }

    void update_stats() {
        stats.clear();
        update_stats(nodes[0]);
        sort(stats.begin(), stats.end());
    }

    void print() {
        print(nodes[0], "");
    }

    void print(Node * v, string prefix) {
        if (v->is_terminal)
            for (auto pattern_num : v->pattern_num)
                cout << pattern_num << ": " << prefix << "\n";
        for (int i = 0; i < k; ++i) {
            if (v->next[i] != nullptr) {
                print(v->next[i], prefix + string(1, 'a' + i));
            }
        }
    }

    void print_info() {
        for (int i = 0; i < nodes.size(); ++i){
            Node * u = nodes[i];
            cout << "(" << u->inchar << ")-> " << i << " ->";
            for (int j = 0; j < k; ++j) {
                if (u->next[j] != nullptr)
                    cout << "(" << string(1, j + 'a') << ")";
            }
            if (u->suff_link != nullptr)
                cout << " [" << u->suff_link->index << "]\n";
            else
                cout << " [ ]\n";
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    /*
    Example of usage:

    Trie trie = Trie();
    trie.add("she", 1);
    trie.add("he", 2);
    trie.add("hers", 3);
    trie.add("his", 4);
    trie.create_suff_links();
    trie.print();
    trie.print_info();
    */

    string text;
    int n;
    Trie trie = Trie();

    cin >> text;
    cin >> n;
    vector<string> patterns(n);
    for (int i = 0; i < n; ++i) {
        cin >> patterns[i];
        trie.add(patterns[i], i);
    }

    trie.create_suff_links();
    for (char ch : text) {
        trie.update_state(ch);
    }

    trie.update_stats();
    for (auto stat : trie.stats) {
        cout << stat.second.second << " " << stat.second.first << "\n";
    }
}
