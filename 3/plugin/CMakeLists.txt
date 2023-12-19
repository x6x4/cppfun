cmake_minimum_required(VERSION 3.5)

project(self_dynamic_load LANGUAGES CXX)
find_package(fmt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

include_directories(.)
add_subdirectory(plugin_internals)
add_subdirectory(plugins)


