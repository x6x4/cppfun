add_library(dynamic_library STATIC source/dynamic_library.cpp include/dynamic_library.h)

target_include_directories(dynamic_library PUBLIC include)
