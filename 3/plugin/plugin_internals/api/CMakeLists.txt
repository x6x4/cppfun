add_library(plugin INTERFACE include/plugin.h)
target_include_directories(plugin INTERFACE include)

add_library(plugin_manager SHARED include/plugin_manager.h source/plugin_manager.cpp)
target_include_directories(plugin_manager PUBLIC include)
target_link_libraries(plugin_manager plugin)
set_property(TARGET plugin_manager PROPERTY POSITION_INDEPENDENT_CODE ON)
