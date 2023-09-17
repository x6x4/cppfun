cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(2)

set (CMAKE_CXX_STANDARD 20)
set (AUX_FLAGS "-O0 -coverage ")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -std=c++20 -Wall -Wextra -Wpedantic")


#  libs 

add_subdirectory(libgen)
add_subdirectory(lib1)
add_subdirectory(lib2)

#  testing

add_subdirectory(unit_tests)

#  dialog application

#add_executable(main ui.cpp)

#target_link_libraries(main res_table)


