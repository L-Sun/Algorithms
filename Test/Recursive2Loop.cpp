#include <iostream>
#include <stack>
#include "testTool.hpp"

using namespace std;

int P(int n, int x) {
    if (n == 0)
        return 1;
    else if (n == 1)
        return 2 * x;
    else
        return 2 * x * P(n - 1, x) - 2 * (n - 1) * P(n - 2, x);
}

int P2(int n, int x) {
    if (n == 0) return 1;
    if (n == 1) return 2 * x;

    struct Data {
        int id;
        int val;
    };
    stack<Data> s;
    // 填充栈
    for (int i = n; i >= 2; i--) {
        s.push({i, 0});
    }
    s.push({1, 2 * x});
    s.push({0, 1});

    // 由栈顶开始计算
    Data dn_1, dn_2;
    dn_2 = s.top();
    s.pop();
    while (s.size() != 1) {
        dn_1 = s.top();
        s.pop();
        s.top().val = 2 * x * dn_1.val - 2 * (s.top().id - 1) * dn_2.val;
        dn_2        = dn_1;
    }
    return s.top().val;
}

int main(int argc, char const *argv[]) {
    timeFuncInvocation(P2, 35, 3);
    timeFuncInvocation(P, 35, 3);
    return 0;
}
