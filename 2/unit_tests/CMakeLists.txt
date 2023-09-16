find_package(Catch2 2 REQUIRED)

#add_executable(test1 test_res1.cpp)
add_executable(test2 test_res2.cpp)

#target_link_libraries (test1 res Catch2::Catch2)
target_link_libraries (test2 res_table Catch2::Catch2)



