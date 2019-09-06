#include <vector>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "LinearList/Stack.hpp"

namespace {
Stack<int, 10> s;

TEST(StackTest, PushTest) {
    std::vector<int> v;
    for (size_t i = 0; i < 5; i++) {
        s.push(i);
        v.push_back(s.pop());
    }
    for (size_t i = 5; i < 10; i++) {
        s.push(i);
    }
    while (!s.empty()) {
        v.push_back(s.pop());
    }

    EXPECT_THAT(v, ::testing::ContainerEq(
                       std::vector<int>{0, 1, 2, 3, 4, 9, 8, 7, 6, 5}));
}
}  // namespace

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}