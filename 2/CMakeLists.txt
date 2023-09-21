cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_COMPILER "/usr/bin/g++")

project(2)

set (CMAKE_CXX_STANDARD 20)
set (COV_FLAGS "-O0 -coverage ")

#  debug config 
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wno-unused-parameter -std=c++20 -Wall -Wextra -Wpedantic")

#  coverage config
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COV_FLAGS} -std=c++20 -Wall -Wextra -Wpedantic")


#  libs 
add_subdirectory(lib1)
add_subdirectory(lib2)
add_subdirectory(libgen)

#  testing
add_subdirectory(unit_tests)

