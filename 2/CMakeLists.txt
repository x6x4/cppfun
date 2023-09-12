cmake_minimum_required(VERSION 3.16)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -Wpedantic")

add_library(lib_res.o lib/lib_res.h lib/lib_res.cpp)

link_libraries(lib_res.o)

add_executable(main ui.cpp)
