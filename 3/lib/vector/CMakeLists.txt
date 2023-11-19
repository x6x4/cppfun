find_package(Catch2 2 REQUIRED)
find_package(fmt)

add_executable(test_vec test_vec.cpp)

target_link_libraries (test_vec Catch2::Catch2 fmt::fmt)
#add_subdirectory(./vec)