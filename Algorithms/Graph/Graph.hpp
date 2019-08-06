#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>

#include <vector>
#include <list>
#include <map>
#include <stack>

#include <queue>
#include <functional>

template <typename T>
class DFS;

typedef std::vector<std::list<int>> AdjTable;

template <typename T>
class Graph;

template <typename T>
std::stack<T> cycle(const Graph<T>& g);

template <typename T>
class Graph {
    friend std::stack<T> cycle<>(const Graph<T>&);
    friend class DFS<T>;

public:
    Graph() = default;
    Graph(const int& verteices) {
        mAdj    = AdjTable(verteices);
        mSymbol = std::vector<T>(verteices);
    }
    Graph(const std::string& file_name) {
        std::ifstream file(file_name);
        initializeSymbol(file);
        T v, w;
        while (file) {
            file >> v >> w;
            addEdge(mKey[v], mKey[w]);
        }
        file.close();
    }
    ~Graph() {}

    inline int V() const { return mAdj.size(); }
    inline int E() const { return mEdge; }

    void addEdge(const int& v, const int& w) {
        mAdj[v].push_front(w);
        mAdj[w].push_front(v);
        mEdge++;
    }
    // Give all vertices adjacent to v
    const std::list<int>& adj(const int& v) const { return mAdj[v]; }

    // Give adjust table
    std::map<T, std::list<T>> table() const {
        std::map<T, std::list<T>> ret;
        for (auto&& s : mKey) {
            std::list<T> temp;
            for (auto&& i : mAdj[s.second]) temp.push_back(mSymbol[i]);
            ret[s.first] = temp;
        }
        return ret;
    }

    // Is linked between v and w
    bool isLinked(const T& v, const T& w) {
        std::vector<bool> marked(V(), false);
        std::vector<int>  edgeTo(V());
        // dfs
        dfs(mKey[v], marked, edgeTo);
        return marked[mKey[w]];
    }

    std::list<int> path(const T& v, const T& w) {
        std::vector<bool> marked(V(), false);
        std::vector<int>  edgeTo(V());

        dfs(mKey[v], marked, edgeTo);
        std::list<T> ret;
        if (marked[mKey[w]]) {
            for (int i = mKey[w]; i != mKey[v]; i = edgeTo[i])
                ret.push_front(mSymbol[i]);
            ret.push_front(v);
        }
        return ret;
    }

    std::list<int> minPath(const T& v, const T& w) {
        std::vector<bool> marked(V(), false);
        std::vector<int>  edgeTo(V());
        std::list<T>      ret;
        bfs(mKey[v], marked, edgeTo);

        if (marked[mKey[w]]) {
            for (int i = mKey[w]; i != mKey[v]; i = edgeTo[i])
                ret.push_front(mSymbol[i]);
            ret.push_front(v);
        }
        return ret;
    }

    std::map<T, int> mKey;
    std::vector<T>   mSymbol;

protected:
    AdjTable mAdj;
    int      mEdge = 0;

    void initializeSymbol(std::ifstream& file) {
        int count = 0;
        file >> count;
        mAdj      = AdjTable(count);
        mSymbol   = std::vector<T>(count);
        auto last = file.tellg();
        T    v, w;
        int  i = 0;
        while (file) {
            file >> v >> w;
            if (mKey.find(v) == mKey.end()) {
                mKey[v]          = i++;
                mSymbol[mKey[v]] = v;
            }
            if (mKey.find(w) == mKey.end()) {
                mKey[w]          = i++;
                mSymbol[mKey[w]] = w;
            }
        }
        file.clear();
        file.seekg(last);
    }

    // Depth First Search
    void dfs(const int& v, std::vector<bool>& marked,
             std::vector<int>& edgeTo) const {
        marked[v] = true;
        for (auto&& w : mAdj[v]) {
            if (!marked[w]) {
                edgeTo[w] = v;
                dfs(w, marked, edgeTo);
            }
        }
    }

    // Breadth First Search
    void bfs(const int& v, std::vector<bool>& marked,
             std::vector<int>& edgeTo) const {
        std::queue<T> q;
        marked[v] = true;
        q.push(v);
        while (!q.empty()) {
            for (auto&& i : mAdj[q.front()]) {
                if (!marked[i]) {
                    marked[i] = true;
                    q.push(i);
                    edgeTo[i] = q.front();
                }
            }
            q.pop();
        }
    }
};

// Calculate degree of a vertex
template <typename T>
int degree(const Graph<T>& g, T v) {
    return g.adj(v).size();
}

// Get the vertex which of the degree is max
template <typename T>
int maxDegree(const Graph<T>& g) {
    int ret = 0;
    for (int v = 0; v < g.V(); v++)
        if (ret < degree(g, v)) ret = degree(g, v);
    return ret;
}

// Calculate the average degree of a Graph
template <typename T>
int avgDegree(const Graph<T>& g) {
    return 2 * g.E() / g.V();
}

template <typename T>
std::stack<T> cycle(const Graph<T>& g) {
    std::vector<bool> marked(g.V());
    std::vector<bool> onStack(g.V());
    std::vector<int>  edgeTo(g.V());
    std::stack<int>   cycle;
    bool              hasCycle = false;

    std::function<void(const int&)> dfs = [&](const int& v) {
        onStack[v] = true;
        marked[v]  = true;
        for (auto&& w : g.adj(v)) {
            if (hasCycle)
                return;
            else if (!marked[w]) {
                edgeTo[w] = v;
                dfs(w);
            } else if (onStack[w]) {
                hasCycle = true;
                for (int i = v; i != w; i = edgeTo[i]) cycle.push(g.mSymbol[i]);
                cycle.push(g.mSymbol[w]);
                cycle.push(g.mSymbol[v]);
            }
        }
        onStack[v] = false;
    };
    for (size_t i = 0; i < g.V(); i++)
        if (!marked[i]) dfs(i);

    return cycle;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Graph<T>& G) {
    out << G.V() << " vertices, " << G.E() << " edges.";
    for (auto&& v : G.table()) {
        out << std::endl << std::setw(3) << v.first << " :";
        for (auto&& w : v.second) out << " " << std::setw(3) << w;
    }
    return out;
}
