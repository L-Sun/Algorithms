#include "Graph/EdgeWeightedDigraph.hpp"
#include "testTool.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    EdgeWeightedDigraph<int> ewd("Asset/text/tinyEWD.txt");
    cout << ewd << endl;

    SP<int> sp(ewd, 0);
    cout << sp.pathTo(6) << endl;

    return 0;
}
