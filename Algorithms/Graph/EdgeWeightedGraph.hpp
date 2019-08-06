#pragma once

#include <vector>
#include <list>
#include <map>
#include <queue>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <functional>

struct Edge {
    Edge() = default;
    Edge(int v, int w, double weight) : v(v), w(w), weight(weight) {}

    int    v, w;
    double weight;

    int other(int x) {
        if (x == v)
            return w;
        else if (x == w)
            return v;
        else
            throw;
    }

    bool operator>(const Edge& rhs) { return weight > rhs.weight; }
    bool operator<(const Edge& rhs) { return weight < rhs.weight; }
    bool operator==(const Edge& rhs) { return weight == rhs.weight; }
};
typedef std::shared_ptr<Edge> pEdge;

template <typename T>
class EdgeWeightedGraph {
public:
    EdgeWeightedGraph(int V) {
        mSymbol = std::vector<T>(V);
        mAdj    = std::vector<std::list<pEdge>>(V);
    };

    EdgeWeightedGraph(const std::string file_name) {
        std::ifstream file(file_name);
        int           vertices, edges;
        file >> vertices >> edges;
        mSymbol = std::vector<T>(vertices);
        mAdj    = std::vector<std::list<std::shared_ptr<Edge>>>(vertices);
        initializeSymbol(file);
        T      v, w;
        double weight;
        while (file) {
            file >> v >> w >> weight;
            pEdge e = std::make_shared<Edge>(v, w, weight);
            addEdge(e);
        }
        file.close();
    }

    const std::list<pEdge>& adj(int v) const { return mAdj[v]; }
    void                    addEdge(const pEdge& e) {
        mAdj[e->v].push_back(e);
        mAdj[e->w].push_back(e);
        mEdge++;
    }

    inline int                     V() const { return mAdj.size(); }
    inline int                     E() const { return mEdge; }
    inline const std::map<T, int>& keys() const { return mKey; }
    inline const std::vector<T>&   symbols() const { return mSymbol; }
    inline const T&                symbol(int id) const { return mSymbol[id]; }
    inline int id(const T& symbol) const { return mKey.at(symbol); }

    friend std::ostream& operator<<(std::ostream&            out,
                                    const EdgeWeightedGraph& ewg) {
        for (std::pair<T, int>&& s : ewg.mKey)
            for (const pEdge& p : ewg.mAdj[s.second])
                if (p->v == s.second)
                    out << s.first << "-" << ewg.mSymbol[p->w] << ": "
                        << p->weight << std::endl;

        return out;
    }

private:
    std::map<T, int>              mKey;
    std::vector<T>                mSymbol;
    std::vector<std::list<pEdge>> mAdj;
    int                           mEdge;

    void initializeSymbol(std::ifstream& file) {
        auto   last = file.tellg();
        T      v, w;
        double weight;
        int    i = 0;
        while (file) {
            file >> v >> w >> weight;
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
};

template <typename T>
class LazzyPrimMST {
public:
    LazzyPrimMST(const EdgeWeightedGraph<T>& g)
        : mSymbols(g.symbols()), mKeys(g.keys()) {
        marked = std::vector<bool>(g.V());

        int curr = g.keys().cbegin()->second;
        do {
            marked[curr] = true;
            for (const pEdge& e : g.adj(curr))
                if (!marked[e->other(curr)]) {
                    mQ.push(e);
                }

            const pEdge e = mQ.top();
            mQ.pop();
            int v = e->v, w = e->w;
            if (marked[v] && marked[w]) continue;
            mst.push(e);
            curr = marked[v] ? w : v;
        } while (!mQ.empty());
    }
    friend std::ostream& operator<<(std::ostream&       out,
                                    const LazzyPrimMST& mst) {
        std::queue<pEdge> mst_queue = mst.mst;
        while (!mst_queue.empty()) {
            const Edge& e = *mst_queue.front();
            out << e.v << "-" << e.w << ":" << e.weight << std::endl;
            mst_queue.pop();
        }
        return out;
    }

private:
    std::function<bool(const pEdge&, const pEdge&)> cmp =
        [](const pEdge& lhs, const pEdge& rhs) { return *lhs > *rhs; };

    std::priority_queue<pEdge, std::vector<pEdge>, decltype(cmp)> mQ{cmp};

    const std::map<T, int>& mKeys;
    const std::vector<T>&   mSymbols;
    std::vector<bool>       marked;
    std::queue<pEdge>       mst;
};
