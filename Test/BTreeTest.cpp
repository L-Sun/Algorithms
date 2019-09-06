#include "Tree/BTree.hpp"

int main(int argc, char const *argv[]) {
    BTree<int> t("./Asset/text/tree.txt");
    t.print("pre");
    t.print("in");
    t.print("post");

    t.print("pre2");
    t.print("in2");
    t.print("post2");

    return 0;
}
