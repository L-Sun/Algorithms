#include <iostream>

using namespace std;

template <typename T, size_t D>
void print_array(T (&a)[D]) {
    cout << "[ ";
    for (size_t i = 0; i < D; i++) {
        cout << a[i] << ", ";
    }
    cout << "]" << endl;
}

template <size_t D>
bool delRange(int (&a)[D], int s, int t) {
    if (s >= t || D == 0) return false;
    int k = 0;
    for (int i = 0; i < D; i++) {
        if (a[i] < s || a[i] > t) {
            a[k] = a[i];
            k++;
        }
    }
    return true;
}

template <size_t D>
void delRepeat(int (&a)[D]) {
    if (D == 0 && D == 1) return;
    int i = 0, j = 00;
    while (j < D) {
        if (a[i] != a[j]) {
            a[++i] = a[j];
        }
        j++;
    }
    cout << i << endl;
}

template <size_t D>
bool findMainElement(int (&a)[D], int& e) {
    static_assert(D > 0, "array size is not postive");
    e       = a[0];
    int cnt = 0;
    for (size_t i = 0; i < D; i++) {
        e == a[i] ? cnt++ : cnt--;
        if (cnt == 0) e = a[i + 1];
    }
    // verify the main element
    cnt = 0;
    for (size_t i = 0; i < D; i++)
        if (e == a[i]) cnt++;

    return cnt > (D - 1) / 2;
}

int main() {
    int data[] = {0, 5, 5, 3, 5, 1, 5, 7};
    int e;
    if (findMainElement(data, e))
        cout << e << endl;
    else
        cout << "could not find the main element" << endl;

    return 0;
}
