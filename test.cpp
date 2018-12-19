#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "Algorithms/BST.hpp"

using namespace std;
using namespace chrono;

auto timeFuncInvocation = [](auto&& func, auto&&... params) {
    auto start = steady_clock::now();

    std::forward<decltype(func)>(func)(
        std::forward<decltype(params)>(params)...);

    auto end   = steady_clock::now();
    auto delta = duration_cast<milliseconds>(end - start);

    cout << delta.count() << " ms total." << endl;
};

template <class S>
void build_table() {
    S symbol_table;

    fstream fsm("./Asset/text/tal.txt");

    while (!fsm.eof()) {
        string data, temp;
        fsm >> temp;
        for (unsigned char c : temp)
            if (!ispunct(c)) data += c;
        auto value = symbol_table.get(data);
        symbol_table.insert(data, value + 1);
    }
}

int main(int argc, char* argv[]) {
    timeFuncInvocation(build_table<BST<string, unsigned>>);
    return 0;
}
