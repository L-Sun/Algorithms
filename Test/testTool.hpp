#include <iostream>
#include <chrono>
#include <list>
#include <stack>
#include <queue>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& l) {
    auto p = l.cbegin();
    if (p != l.cend()) {
        out << *p++;
        while (p != l.cend()) out << "->" << *p++;
    }
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, std::stack<T> s) {
    if (s.empty()) return out;
    out << s.top();
    s.pop();
    while (!s.empty()) {
        out << ", " << s.top();
        s.pop();
    }
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, std::queue<T> q) {
    if (q.empty()) return out;
    out << q.front();
    q.pop();
    while (!q.empty()) {
        out << ", " << q.front();
        q.pop();
    }
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<const T>& l) {
    auto p = l.cbegin();
    if (p != l.cend()) {
        out << *p++;
        while (p != l.cend()) out << "->" << *p++;
    }
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, std::stack<const T> s) {
    if (s.empty()) return out;
    out << s.top();
    s.pop();
    while (!s.empty()) {
        out << ", " << s.top();
        s.pop();
    }
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, std::queue<const T> q) {
    if (q.empty()) return out;
    out << q.front();
    q.pop();
    while (!q.empty()) {
        out << ", " << q.front();
        q.pop();
    }
    return out;
}

template <typename F, typename... Args>
void timeFuncInvocation(F&& func, Args&&... params) {
    auto start = std::chrono::steady_clock::now();

    std::forward<decltype(func)>(func)(
        std::forward<decltype(params)>(params)...);

    auto end = std::chrono::steady_clock::now();
    auto delta =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << delta.count() << " ms total." << std::endl;
};
