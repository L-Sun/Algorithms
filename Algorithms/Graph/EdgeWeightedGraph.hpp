#pragma once

#include <vector>
#include <list>
#include <map>
#include <queue>
#include <set>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <functional>
#include <limits>

#include "UF.hpp"

template <typename T>
class EdgeWeightedGraph;

class Edge {
public:
    Edge() = default;
    Edge(int v, int w, double weight) : mV(v), mW(w), mWeight(weight) {}

    int other(int x) {
        if (x == mV)
            return mW;
        else if (x == mW)
            return mV;
        else
            throw;
    }
    inline double weight() const { return mWeight; }
    inline int    v() const { return mV; }
    inline int    w() const { return mW; }

    bool operator>(const Edge& rhs) { return mWeight > rhs.mWeight; }
    bool operator<(const Edge& rhs) { return mWeight < rhs.mWeight; }
    bool operator==(const Edge& rhs) { return mWeight == rhs.mWeight; }

private:
    int    mV, mW;
    double mWeight;
};
typedef std::shared_ptr<Edge> pEdge;

template <typename T>
class EdgeWeightedGraph {
public:
    EdgeWeightedGraph(int V) {
        mSymbols = std::vector<T>(V);
        mAdj     = std::vector<std::list<pEdge>>(V);
    };

    EdgeWeightedGraph(const std::string file_name) {
        std::ifstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "Can not open file: " << file_name << std::endl;
            throw;
        };

        int vertices, edges;
        file >> vertices >> edges;
        mSymbols = std::vector<T>(vertices);
        mAdj     = std::vector<std::list<std::shared_ptr<Edge>>>(vertices);
        initializeSymbol(file);
        T      v, w;
        double weight;
        while (file) {
            file >> v >> w >> weight;
            pEdge e = std::make_shared<Edge>(mKeys[v], mKeys[w], weight);
            addEdge(e);
        }
        file.close();
    }

    const std::list<pEdge>& adj(int v) const { return mAdj[v]; }
    void                    addEdge(const pEdge& e) {
        mAdj[e->v()].push_back(e);
        mAdj[e->w()].push_back(e);
        mEdge++;
    }

    inline int                     V() const { return mAdj.size(); }
    inline int                     E() const { return mEdge; }
    inline const std::map<T, int>& keys() const { return mKeys; }
    inline const std::vector<T>&   symbols() const { return mSymbols; }
    inline const T&                symbol(int id) const { return mSymbols[id]; }
    inline int id(const T& symbol) const { return mKeys.at(symbol); }

    friend std::ostream& operator<<(std::ostream&            out,
                                    const EdgeWeightedGraph& ewg) {
        out << "====== Edge Weight Graph ======" << std::endl;
        out << "Vertices: " << ewg.V() << " Edge: " << ewg.E() << std::endl;
        for (std::pair<T, int>&& s : ewg.mKeys)
            for (const pEdge& p : ewg.mAdj[s.second])
                if (p->v() == s.second)
                    out << s.first << "-" << ewg.mSymbols[p->w()] << ": "
                        << p->weight() << std::endl;

        return out;
    }

private:
    std::map<T, int>              mKeys;
    std::vector<T>                mSymbols;
    std::vector<std::list<pEdge>> mAdj;
    int                           mEdge;

    void initializeSymbol(std::ifstream& file) {
        auto   last = file.tellg();
        T      v, w;
        double weight;
        int    i = 0;
        while (file) {
            file >> v >> w >> weight;
            if (mKeys.find(v) == mKeys.end()) {
                mKeys[v]           = i++;
                mSymbols[mKeys[v]] = v;
            }
            if (mKeys.find(w) == mKeys.end()) {
                mKeys[w]           = i++;
                mSymbols[mKeys[w]] = w;
            }
        }
        file.clear();
        file.seekg(last);
    }
};

class LazzyPrimMST {
public:
    template <typename T>
    LazzyPrimMST(const EdgeWeightedGraph<T>& ewg) {
        std::vector<bool> marked(ewg.V(), false);

        std::function<bool(const pEdge&, const pEdge&)> cmp =
            [](const pEdge& lhs, const pEdge& rhs) { return *lhs > *rhs; };
        std::priority_queue<pEdge, std::vector<pEdge>, decltype(cmp)> pq(cmp);

        int curr = ewg.keys().cbegin()->second;
        do {
            marked[curr] = true;
            for (const pEdge& e : ewg.adj(curr))
                if (!marked[e->other(curr)]) {
                    pq.push(e);
                }

            pEdge e = pq.top();
            pq.pop();
            int v = e->v(), w = e->w();
            if (marked[v] && marked[w]) continue;
            mst.push(e);
            curr = marked[v] ? w : v;
        } while (!pq.empty());

        std::queue<pEdge> _mst = mst;
        while (!_mst.empty()) {
            pEdge& e = _mst.front();
            ss << ewg.symbol(e->v()) << "-" << ewg.symbol(e->w()) << ":"
               << e->weight() << std::endl;
            _mst.pop();
        }
    }

    friend std::ostream& operator<<(std::ostream&       out,
                                    const LazzyPrimMST& mst) {
        return out << "Lazzy Prim MST:\n" << mst.ss.str();
    }

private:
    std::queue<pEdge> mst;
    std::stringstream ss;
};

class PrimMST {
public:
    template <typename T>
    PrimMST(const EdgeWeightedGraph<T>& ewg)
        : edgeTo(ewg.V()), distTo(ewg.V()) {
        auto cmp = [&](const int& lhs, const int& rhs) {
            return distTo[lhs] < distTo[rhs];
        };
        std::set<int, decltype(cmp)> pq(cmp);
        std::vector<bool>            marked(ewg.V(), false);

        for (double& i : distTo) {
            i = std::numeric_limits<double>::max();
        }

        int curr     = ewg.keys().cbegin()->second;
        distTo[curr] = 0.0;
        edgeTo[curr] = curr;
        pq.insert(curr);

        while (!pq.empty()) {
            // pop the first vertex
            curr = *pq.begin();
            pq.erase(pq.begin());

            marked[curr] = true;
            for (const pEdge& e : ewg.adj(curr)) {
                int v = e->other(curr);
                if (marked[v]) continue;
                if (e->weight() < distTo[v]) {
                    edgeTo[v] = curr;
                    // find the edge having vertex v;
                    auto find_v = pq.find(v);
                    if (find_v != pq.end()) {
                        pq.erase(find_v);
                    }
                    // update distTo[v]
                    distTo[v] = e->weight();
                    pq.insert(v);
                }
            }
        }

        for (size_t i = 0; i < edgeTo.size(); i++) {
            if (i == edgeTo[i]) continue;
            ss << ewg.symbol(i) << "-" << ewg.symbol(edgeTo[i]) << ":"
               << distTo[i] << std::endl;
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const PrimMST& mst) {
        return out << "Prim MST:\n" << mst.ss.str();
    }

private:
    std::vector<int>    edgeTo;
    std::vector<double> distTo;
    std::stringstream   ss;
};

class Kruskal {
public:
    template <typename T>
    Kruskal(const EdgeWeightedGraph<T>& ewg) {
        auto cmp = [](const pEdge& lhs, const pEdge& rhs) -> bool {
            return lhs->weight() > rhs->weight();
        };
        std::priority_queue<pEdge, std::vector<pEdge>, decltype(cmp)> pq(cmp);
        UF uf(ewg.V());

        // construct a sorted priority queue of edge weight
        for (auto&& i : ewg.keys())
            for (auto&& e : ewg.adj(i.second))
                if (i.second == e->v()) pq.push(e);

        // 1 is red, -1 is black, 0 is not marked
        while (!pq.empty() && mst.size() < ewg.V() - 1) {
            pEdge p = pq.top();
            pq.pop();
            int v = p->v(), w = p->w();
            if (uf.isConnected(v, w)) continue;
            uf.weigthUnionMerge(v, w);
            mst.push_back(p);
        }

        for (auto&& e : mst) {
            ss << ewg.symbol(e->v()) << "-" << ewg.symbol(e->w()) << ":"
               << e->weight() << std::endl;
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const Kruskal& mst) {
        return out << "Kruskal MST: \n" << mst.ss.str();
    }

private:
    std::vector<pEdge> mst;
    std::stringstream  ss;
};