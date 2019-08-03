#pragma once
#include "Graph.hpp"

template <typename T>
class DiGraph : public Graph<T> {
public:
    DiGraph(const int& verteices) : Graph<T>(verteices) {}
    DiGraph(const std::string& file_name) {
        std::ifstream file(file_name);
        Graph<T>::initializeSymbol(file);

        T v, w;
        while (file) {
            file >> v >> w;
            addEdge(Graph<T>::mKey[v], Graph<T>::mKey[w]);
        }
        file.close();
    }

    DiGraph reverse() const {
        DiGraph R = *this;
        R.mEdge   = 0;
        R.mAdj    = AdjTable(Graph<T>::V());
        for (auto&& s : Graph<T>::mKey)
            for (auto&& i : Graph<T>::mAdj[s.second]) R.addEdge(i, s.second);

        return R;
    }

    void addEdge(const int& from, const int& to) {
        Graph<T>::mAdj[from].push_front(to);
        Graph<T>::mEdge++;
    }
};
