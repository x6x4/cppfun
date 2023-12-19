add_library(library_manager STATIC source/library_manager.cpp include/library_manager.h)
target_link_libraries(library_manager dynamic_library plugin)

target_include_directories(library_manager PUBLIC include)
