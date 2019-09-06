#pragma once
#include <iostream>
#include <exception>

template <typename T, int N>
class Stack {
private:
    T   mData[N];
    int mTop = -1;

public:
    Stack() {}

    T& top() { return mData[mTop]; }

    T& pop() {
        if (mTop < 0) throw std::runtime_error("Stack is Empty!");
        return mData[mTop--];
    }
    void push(const T& e) {
        if (mTop >= N) throw std::runtime_error("Stack is Full");
        mData[++mTop] = e;
    }
    bool empty() const { return mTop == -1; }
};
