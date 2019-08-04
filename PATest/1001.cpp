#include <iostream>
#include <stack>
#include <iomanip>

using namespace std;

int main(int argc, char const *argv[]) {
    int a, b;

    cin >> a >> b;
    int sum = a + b;
    if (sum == 0) {
        cout << sum;
        return 0;
    }

    bool isNegative = sum < 0;
    sum             = abs(sum);

    stack<unsigned> output;
    while (sum > 0) {
        output.push(sum % 1000);
        sum /= 1000;
    }

    if (isNegative) cout << "-";
    bool isFirst = true;
    while (!output.empty()) {
        if (isFirst) {
            cout << output.top();
            isFirst = false;
        } else {
            cout << setw(3) << setfill('0') << output.top();
        }
        if (output.size() != 1) cout << ",";
        output.pop();
    }
    return 0;
}
