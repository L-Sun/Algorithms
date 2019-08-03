#pragma once

#include <vector>
#include <list>
#include <map>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>

struct Edge {
    Edge() = default;
    Edge(int v, int w, double weight) : v(v), w(w), weight(weight) {}

    int    v, w;
    double weight;

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

    const std::list<Edge> adj(const T& v) const { return mAdj[v]; }
    void                  addEdge(const pEdge& e) {
        mAdj[e->v].push_back(e);
        mAdj[e->w].push_back(e);
        mEdge++;
    }

    inline int V() const { return mAdj.size(); }
    inline int E() const { return mEdge; }

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
