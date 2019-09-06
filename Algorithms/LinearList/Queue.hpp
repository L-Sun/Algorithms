#pragma once
#include <list>

template <typename T>
class Queue {
public:
    Queue() = default;
    void enqueue(T e) { mQ.push_back(e); }
    T    dequeue() {
        T result = mQ.front();
        mQ.pop_front();
        return result;
    }
    bool empty() const { return mQ.empty(); }

private:
    std::list<T> mQ;
};