find_package(Catch2 2 REQUIRED)

add_executable(test test_res.cpp)

target_link_libraries (test res Catch2::Catch2)



