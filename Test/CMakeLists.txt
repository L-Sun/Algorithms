add_executable(StackTest StackTest.cpp)
target_link_libraries(StackTest ${GTEST_BOTH_LIBRARIES})
gtest_add_tests(TARGET StackTest)


add_executable(BSTTest BSTTest.cpp)
add_executable(ListTest ListTest.cpp)
add_executable(GraphTest GraphTest.cpp)
add_executable(DiGraphTest DiGraphTest.cpp)
add_executable(EdgeWeightedGraphTest EdgeWeightedGraphTest.cpp)
add_executable(EdgeWeightedDigraphTest EdgeWeightedDigraphTest.cpp)
add_executable(Recursive2Loop Recursive2Loop.cpp)