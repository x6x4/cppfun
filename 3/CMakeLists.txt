cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_COMPILER "/usr/bin/g++")

project(3)

set (CMAKE_CXX_STANDARD 20)
set (AUX_FLAGS "-O0 -coverage ")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wno-unused-parameter -std=c++20 -Wall -Wextra -Wpedantic")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -coverage -Wno-unused-parameter -std=c++20 -Wall -Wextra -Wpedantic")

#  libs 

add_subdirectory(lib/prog)
add_subdirectory(lib/mem)

#  testing

#add_subdirectory(tests)

#  dialog application

add_executable(main main.cpp)

target_link_libraries(main mem)


