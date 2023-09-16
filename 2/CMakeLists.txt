cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -ggdb -Wall -Wextra -Wpedantic")

add_subdirectory(lib1)
add_subdirectory(lib2)

add_subdirectory(unit_tests)

#add_executable(main ui.cpp)

#target_link_libraries(main res_table)


