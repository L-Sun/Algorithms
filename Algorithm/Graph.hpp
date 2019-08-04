#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Vertex {
    string label      = "";
    bool   bIsVisited = false;
};

class Graph {
public:
    void AddVertex(const Vertex& v) {
        mVertices.push_back(v);
        mAdjList.push_back(list<size_t>());
    }

    void AddEdge(size_t id1, size_t id2) {
        if (hasVertex(id1) && hasVertex(id2)) {
            mAdjList[id1].push_back(id2);
            mAdjList[id2].push_back(id1);
            mEdges++;
        } else
            cout << "Error id" << endl;
    }

    void addWeigh(size_t id1, size_t id2) {
        if (hasVertex(id1) && hasVertex(id2) && hasEdge(id1, id2)) {
            
        }
    }

    friend ostream& operator<<(ostream& os, const Graph& g) {
        os << "Vertices: " << setw(5) << g.mVertices.size()
           << "Edge: " << setw(5) << endl;
        for (size_t i = 0; i < g.mVertices.size(); i++) {
            os << setw(2) << i << "-> ";
            string comma = "";
            for (auto vAdjId : g.mAdjList[i]) {
                os << comma << vAdjId;
                comma = ", ";
            }
            cout << endl;
        }
        return os;
    }

private:
    vector<Vertex>       mVertices;
    vector<list<size_t>> mAdjList;
    unsigned             mEdges;

    bool hasVertex(size_t id) const { return id < mVertices.size(); }
    bool hasEdge(size_t id1, size_t id2) const {
        return find(mAdjList[id1].cbegin(), mAdjList[id1].cend(), id2) !=
               mAdjList[id1].cend();
    }
};