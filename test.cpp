#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <random>
#include <iomanip>
#include <cmath>

#include "Algorithms/BST.hpp"

using namespace std;
using namespace chrono;

template <typename F, typename... Args>
void timeFuncInvocation(F&& func, Args&&... params) {
    auto start = steady_clock::now();

    std::forward<decltype(func)>(func)(
        std::forward<decltype(params)>(params)...);

    auto end   = steady_clock::now();
    auto delta = duration_cast<milliseconds>(end - start);

    cout << delta.count() << " ms total." << endl;
};

template <class S>
void build_table(const string& input, const string& output) {
    S symbol_table;

    ifstream ifsm(input);
    ofstream ofsm(output);

    while (!ifsm.eof()) {
        string data, temp;
        ifsm >> temp;
        for (unsigned char c : temp)
            if (!ispunct(c)) data += c;
        auto value = symbol_table.get(data);
        symbol_table.insert(data, value + 1);
    }
}

template <class S>
void build_random_table(size_t N, bool isRandom) {
    cout << "-------" << endl;
    cout << "Random: " << (isRandom ? "ON" : "OFF") << endl;
    cout << "N    =" << setw(8) << left << N << endl;

    S                                st;
    random_device                    r;
    default_random_engine            e(r());
    uniform_int_distribution<size_t> dist1(0, SIZE_MAX);
    uniform_int_distribution<int>    dist2(0, N - 1);
    while (st.size() != N) st.insert(dist1(e), dist1(e));

    st.vertify();
    while (st.size() < N) st.insert(dist1(e), dist1(e));
    ofstream ofsm("./Asset/text/x.txt");
    for (size_t i = 0; i < N * N; i++) {
        int  rank = dist2(e);
        auto key  = st.select(rank);
        auto x    = st.size();
        auto v    = st.get(key);
        st.del(key, isRandom);
        if (st.size() != x - 1) {
            st.print(ofsm);
            throw;
        }
        key = dist1(e);
        st.insert(key, dist1(e));
        if (st.size() != x) {
            throw;
        }
    }
    cout << "AOIPL=" << setw(8) << left << st.meanInnerPathLen() << endl;
}

int main(int argc, char* argv[]) {
    string input, output;

    input  = argc >= 2 ? argv[1] : "./Asset/text/tal.txt";
    output = argc == 3 ? argv[2] : "./Asset/text/x.txt";

    timeFuncInvocation(build_table<BST<string, unsigned>>, input, output);
    timeFuncInvocation(build_random_table<BST<size_t, size_t>>, std::pow(10, 2),
                       false);
    timeFuncInvocation(build_random_table<BST<size_t, size_t>>, std::pow(10, 2),
                       true);
    timeFuncInvocation(build_random_table<BST<size_t, size_t>>, std::pow(10, 3),
                       false);
    timeFuncInvocation(build_random_table<BST<size_t, size_t>>, std::pow(10, 3),
                       true);
    timeFuncInvocation(build_random_table<BST<size_t, size_t>>, std::pow(10, 4),
                       false);
    timeFuncInvocation(build_random_table<BST<size_t, size_t>>, std::pow(10, 4),
                       true);
    return 0;
}
