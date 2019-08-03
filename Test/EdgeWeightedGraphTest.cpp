#include "EdgeWeightedGraph.hpp"
#include "testTool.hpp"

#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    EdgeWeightedGraph<int> ewg("Asset/text/tinyEWG.txt");
    cout << ewg << endl;
    return 0;
}
