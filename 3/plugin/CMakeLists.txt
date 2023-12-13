cmake_minimum_required(VERSION 3.5)

project(self_dynamic_load LANGUAGES CXX)
find_package(fmt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

include_directories(.)

add_subdirectory(api)
add_subdirectory(dynamic_library)
add_subdirectory(library_manager)
add_subdirectory(plugins)

add_executable(main main.cpp)
target_link_libraries(main fmt::fmt)
target_link_libraries(main plugin_manager)
target_link_libraries(main library_manager)
