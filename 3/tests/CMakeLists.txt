find_package(Catch2 2 REQUIRED)

add_executable(test1 test_cpu.cpp)
target_link_libraries (test1 Catch2::Catch2)





