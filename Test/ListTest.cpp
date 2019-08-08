#include <iostream>
#include "List.h"

using namespace MyAlgorithms;
using namespace std;

int main(int argc, char const* argv[]) {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    List<int> l(a);
    cout << l << endl;

    l.del(1);
    cout << l << endl;
    l.reserve();
    cout << l << endl;

    l.reserve_1();
    cout << l << endl;

    l.sort();
    cout << l << endl;

    l.delBetween(3, 6);
    cout << l << endl;

    List<int> B;
    extractEven(l, B);
    cout << l << endl;
    cout << B << endl;

    List<int> C(a);
    List<int> C1;
    discreate(C, C1);
    cout << C << endl;
    cout << C1 << endl;

    C.sort();
    C1.sort();

    cout << C << endl;
    cout << C1 << endl;

    List<int> M = mergeInverse(C, C1);
    cout << M << endl;

    int       a_12[] = {1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 7, 7, 8};
    List<int> L_12(a_12);
    cout << L_12 << endl;
    L_12.sortAndDelReapte();
    cout << L_12 << endl;

    int       a_13_1[] = {1, 5, 5, 6, 7, 7, 7, 7, 8};
    int       a_13_2[] = {1, 2, 2, 3, 4, 5, 6, 6};
    List<int> L_13_1(a_13_1);
    List<int> L_13_2(a_13_2);
    cout << L_13_1 << endl;
    cout << L_13_2 << endl;
    List<int> L_13_3 = getCommon(L_13_1, L_13_2);
    cout << L_13_3 << endl;

    return 0;
}
