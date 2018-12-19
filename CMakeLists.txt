cmake_minimum_required(VERSION 3.1)
project(Algorithms)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

include_directories("${PROJECT_SOURCE_DIR}/Algorithms")

include(CTest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# enable_testing()

add_executable(MyTest test.cpp)
# target_link_libraries(MyTest ${GTEST_BOTH_LIBRARIES})
# gtest_add_tests(TARGET MyTest)