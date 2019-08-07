#include "Graph/DiGraph.hpp"
#include "testTool.hpp"
#include <iostream>

using namespace std;

template <typename T>
class DFS {
public:
    DFS(const Graph<T>& g) : g(g) {
        marked = std::vector<bool>(g.V());
        for (auto&& i : g.mKey)
            if (!marked[i.second]) dfs(i.second);
    }

    Graph<T>      g;
    std::queue<T> pre;
    std::queue<T> post;
    std::stack<T> reversPost;

    std::vector<bool> marked;

    void dfs(const int& v) {
        marked[v] = true;
        pre.push(g.mSymbol[v]);
        for (auto&& i : g.adj(v))
            if (!marked[i]) dfs(i);
        post.push(g.mSymbol[v]);
        reversPost.push(g.mSymbol[v]);
    };

    friend ostream& operator<<(ostream& out, DFS d) {
        out << "pre: " << d.pre << endl;
        out << "post: " << d.post << endl;
        out << "reversePost: " << d.reversPost << endl;
        return out;
    }
};

template <typename T>
class KosarajuSCC {
public:
    KosarajuSCC(const DiGraph<T>& dg) {
        marked       = vector<bool>(dg.V(), false);
        id           = vector<int>(dg.V());
        DFS<T> order = DFS<T>(dg.reverse());
        cout << order.reversPost << endl;
        while (!order.reversPost.empty()) {
            if (!marked[dg.mKey.at(order.reversPost.top())]) {
                dfs(dg, dg.mKey.at(order.reversPost.top()));
                count++;
            }
            order.reversPost.pop();
        }
    }

    bool stronglyConnected(const T& v, const T& w) { return id[v] == id[w]; }

private:
    vector<bool> marked;
    vector<int>  id;
    int          count = 0;

    void dfs(const DiGraph<T>& dg, const int& v) {
        marked[v]         = true;
        id[dg.mSymbol[v]] = count;
        for (auto&& i : dg.adj(v)) {
            if (!marked[i]) dfs(dg, i);
        }
    }
};

int main(int argc, char const* argv[]) {
    DiGraph<int> dg("Asset/text/tinyDG.txt");
    cout << dg << endl;
    cout << "dg path: " << dg.path(12, 1) << endl;
    cout << "dg min path: " << dg.minPath(12, 1) << endl;
    cout << "dg cycle: " << cycle(dg) << endl;

    DFS<int> dg_dfs_test(DiGraph<int>("Asset/text/tintDAG.txt"));
    cout << dg_dfs_test << endl;

    KosarajuSCC<int> kosaraju_test(dg);
    int              v = 10, w = 11;
    cout << "is strong connected between " << v << " and " << w << ": "
         << (kosaraju_test.stronglyConnected(v, w) ? "true" : "false") << endl;

    return 0;
}
