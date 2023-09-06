cmake_minimum_required(VERSION 3.16)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -Wpedantic")

add_library(libmatrix.o libmatrix.h libmatrix.cpp)

link_libraries(libmatrix.o)

add_executable(main main.cpp)
