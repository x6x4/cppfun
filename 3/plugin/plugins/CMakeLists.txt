add_library(default_set SHARED default_set.cpp)
set_property(TARGET default_set PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(default_set plugin plugin_manager IR operands)

add_library(multithread SHARED multithread.cpp)
set_property(TARGET multithread PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(multithread plugin plugin_manager IR operands)
