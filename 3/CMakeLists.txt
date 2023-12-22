cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_COMPILER "/usr/bin/g++")

project(3)

set (CMAKE_CXX_STANDARD 20)
set (AUX_FLAGS "-O0 -coverage ")

set (CMAKE_CXX_FLAGS "-ggdb -fPIC -Wno-unused-parameter -std=c++20 -Wall -Wextra -Wpedantic")

find_package(fmt)

#  libs 

add_subdirectory(lib)
#add_subdirectory(lib/vector)
add_subdirectory(plugin)
add_subdirectory(gen)

add_executable(main main.cpp)
add_executable(threads threads.cpp)

target_link_libraries(main gen fmt::fmt)
target_link_libraries(threads gen fmt::fmt)



