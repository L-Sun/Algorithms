#pragma once
// Container
#include <vector>
#include <map>
#include <set>
#include <list>

#include <memory>
#include <limits>
#include <functional>

// stream
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class DirectedEdge {
public:
    DirectedEdge(int from, int to, double weight)
        : mFrom(from), mTo(to), mWeight(weight) {}

    inline int    from() const { return mFrom; }
    inline int    to() const { return mTo; }
    inline double weight() const { return mWeight; }
    inline int    other(int x) {
        if (x == mFrom)
            return mTo;
        else if (x == mTo)
            return mFrom;
        else
            throw "error id";
    }
    friend std::ostream& operator<<(std::ostream& out, const DirectedEdge& e) {
        return out << e.from() << "-" << e.to() << " : " << e.weight();
    }

private:
    int    mFrom, mTo;
    double mWeight;
};

typedef std::shared_ptr<DirectedEdge> pDiEdge;

template <typename T>
class EdgeWeightedDigraph {
public:
    EdgeWeightedDigraph(const std::string& file_name) {
        std::ifstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "can open the file: " << file_name << std::endl;
            throw;
        }
        int vertices, edges;
        file >> vertices >> edges;
        mSymbols = std::vector<T>(vertices);
        mAdj     = std::vector<std::list<pDiEdge>>(vertices);

        initializeSymbols(file);
        T      from, to;
        double weight;
        while (file) {
            file >> from >> to >> weight;
            pDiEdge e =
                std::make_shared<DirectedEdge>(mKeys[from], mKeys[to], weight);
            addEdge(e);
        }
    }

    const std::map<T, int>& keys() const { return mKeys; }
    const std::vector<T>&   symbols() const { return mSymbols; }

    int      key(const T& symbol) const { return mKeys.at(symbol); }
    const T& symbol(int key) const { return mSymbols[key]; }

    const std::list<pDiEdge>& adj(int key) const { return mAdj[key]; }

    inline int V() const { return mSymbols.size(); }
    inline int E() const { return mEdges; }

    void addEdge(const pDiEdge& e) {
        mAdj[e->from()].push_back(e);
        mEdges++;
    }

    friend std::ostream& operator<<(std::ostream&              out,
                                    const EdgeWeightedDigraph& ewd) {
        out << "====== Edge Weighted Digraph ======" << std::endl;
        out << "Vertices : " << ewd.V() << " Edges: " << ewd.E() << std::endl;
        for (auto&& i : ewd.keys())
            for (auto&& e : ewd.adj(i.second))
                out << ewd.symbol(e->from()) << "->" << ewd.symbol(e->to())
                    << " : " << e->weight() << std::endl;

        return out;
    }

private:
    std::map<T, int>                mKeys;
    std::vector<T>                  mSymbols;
    std::vector<std::list<pDiEdge>> mAdj;
    int                             mEdges = 0;

    void initializeSymbols(std::ifstream& file) {
        auto   last = file.tellg();
        T      from, to;
        double weight;
        int    count = 0;
        while (file) {
            file >> from >> to >> weight;
            if (mKeys.find(from) == mKeys.end()) {
                mKeys[from]           = count++;
                mSymbols[mKeys[from]] = from;
            }
            if (mKeys.find(to) == mKeys.end()) {
                mKeys[to]           = count++;
                mSymbols[mKeys[to]] = to;
            }
        }
        file.clear();
        file.seekg(last);
    }
};

template <typename T>
class SP {
public:
    SP(const EdgeWeightedDigraph<T>& ewd, const T& s)
        : mEwd(ewd),
          mEdgeTo(ewd.V(), nullptr),
          mDistTo(ewd.V(), std::numeric_limits<double>::max()) {
        mDistTo[mEwd.key(s)] = 0.0;
        indexPQ.insert(mEwd.key(s));

        while (!indexPQ.empty()) {
            int curr = *indexPQ.begin();
            indexPQ.erase(indexPQ.begin());
            relax(curr);
        }
    }

    double distTo(const T& v) { return mDistTo[mEwd.key(v)]; }
    bool   hasPathTo(const T& v) {
        return mDistTo[mEwd.key(v)] < std::numeric_limits<double>::max();
    }
    std::list<const DirectedEdge> pathTo(const T& v) {
        if (!hasPathTo(v)) return std::list<DirectedEdge>();
        std::list<const DirectedEdge> path;
        for (auto e = mEdgeTo[mEwd.key(v)]; e != nullptr; e = mEdgeTo[e]) {
            auto _e = DirectedEdge(mEwd.symbol(e->from()), mEwd.symbol(e->to()),
                                   e->weight());
            path.push_front(_e);
        }

        return path;
    }

private:
    const EdgeWeightedDigraph<T>& mEwd;
    std::stringstream             mSs;
    std::vector<pDiEdge>          mEdgeTo;
    std::vector<double>           mDistTo;

    std::function<bool(int, int)> cmp = [&](int lhs, int rhs) -> bool {
        return mDistTo[lhs] < mDistTo[rhs];
    };
    std::set<int, decltype(cmp)> indexPQ{cmp};

    void relax(int v) {
        for (auto&& e : mEwd.adj(v)) {
            int w = e->to();
            if (mDistTo[w] > mDistTo[v] + e->weight()) {
                mEdgeTo[w]  = e;
                auto find_w = indexPQ.find(w);
                if (find_w != indexPQ.end()) {
                    indexPQ.erase(find_w);
                }
                mDistTo[w] = mDistTo[v] + e->weight();
                indexPQ.insert(w);
            }
        }
    }
};