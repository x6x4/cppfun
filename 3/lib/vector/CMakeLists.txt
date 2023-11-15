find_package(Catch2 2 REQUIRED)

add_executable(test_vec test_vec.cpp)
target_link_libraries (test_vec Catch2::Catch2)
#add_subdirectory(./vec)