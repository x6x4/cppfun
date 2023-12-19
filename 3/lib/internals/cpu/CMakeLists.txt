
find_package(fmt)

SET(LIB_NAME "cpu")

add_library (${LIB_NAME} ${LIB_NAME}.cpp ${LIB_NAME}.h)

target_link_libraries(${LIB_NAME} mem vec fmt)