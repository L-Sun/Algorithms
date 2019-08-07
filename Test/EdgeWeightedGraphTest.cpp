#include "Graph/EdgeWeightedGraph.hpp"
#include "testTool.hpp"

#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    EdgeWeightedGraph<int> ewg("Asset/text/tinyEWG.txt");
    cout << ewg << endl;

    LazzyPrimMST mst1(ewg);
    cout << mst1 << endl;

    PrimMST mst2(ewg);
    cout << mst2 << endl;

    Kruskal mst3(ewg);
    cout << mst3 << endl;

    EdgeWeightedGraph<int> MediumEwg("Asset/text/mediumEWG.txt");

    auto         begin = chrono::steady_clock::now();
    LazzyPrimMST MediumMST1(MediumEwg);
    auto         end = chrono::steady_clock::now();
    auto delta       = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "Meidum EWG MST " << setw(14) << left << "(Lazzy Prim): " << setw(5)
         << std::right << delta.count() << " μs total." << endl;

    begin = chrono::steady_clock::now();
    PrimMST MediumMST2(MediumEwg);
    end   = chrono::steady_clock::now();
    delta = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "Meidum EWG MST " << setw(14) << left << "(Prim): " << setw(5)
         << std::right << delta.count() << " μs total." << endl;

    begin = chrono::steady_clock::now();
    Kruskal MediumMST3(MediumEwg);
    end   = chrono::steady_clock::now();
    delta = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "Meidum EWG MST " << setw(14) << left << "(Kruskal): " << setw(5)
         << std::right << delta.count() << " μs total." << endl;

    return 0;
}
