find_package(Catch2 2 REQUIRED)

add_executable(test1 test_cpu.cpp)
target_link_libraries (test1 cpu Catch2::Catch2)





