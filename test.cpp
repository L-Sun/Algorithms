#include <set>
using namespace std;

int main(int argc, char const *argv[]) {
    set<int> rbTree = {1, 2, 3, 4, 5};
    int      i      = 0;
    for (auto p = rbTree.begin(); p != rbTree.end(); p++, i++) {
        if (i == 3) *p = 10;
    }

    return 0;
}
