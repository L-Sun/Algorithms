cmake_minimum_required(VERSION 3.1)
project(Algorithms)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

include_directories("${PROJECT_SOURCE_DIR}/Algorithms")


file(GLOB pat_files  PATest/*.cpp)
foreach(file  ${pat_files})
    get_filename_component(temp ${file} NAME)
    string(REPLACE ".cpp" "" name ${temp})
    add_executable(${name} ${file})
endforeach(file)
