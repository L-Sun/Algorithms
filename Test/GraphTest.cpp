#include "Graph.hpp"
#include "UF.hpp"
#include "testTool.hpp"
#include <iostream>
#include <list>

using namespace std;

int main(int argc, char const* argv[]) {
    cout << "================ Graph Int TEST ================" << endl;
    Graph<int> gNum("Asset/text/tinyGraph.txt");
    cout << gNum << endl;
    cout << (gNum.isLinked(0, 9) ? "true" : "false") << endl;
    cout << (gNum.isLinked(0, 4) ? "true" : "false") << endl;
    cout << gNum.path(0, 4) << endl;
    cout << gNum.path(2, 3) << endl;
    cout << gNum.minPath(2, 3) << endl;
    cout << "Does the graph have cycle: " << (hasCycle(gNum) ? "true" : "false")
         << endl;

    cout << "================ UF TEST ================" << endl;
    UF guf("Asset/text/tinyGraph.txt");
    cout << (guf.isConnected(2, 3) ? "true" : "false") << endl;

    return 0;
}
