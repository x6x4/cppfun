
SET(LIB_NAME "gen")

add_library (${LIB_NAME} ${LIB_NAME}.cpp ${LIB_NAME}.h)
target_link_libraries(gen IR_compiler plugin_manager library_manager)
