
SET(LIB_NAME "operands")

add_library (${LIB_NAME} ${LIB_NAME}.cpp ${LIB_NAME}.h)

target_link_libraries(${LIB_NAME} cpu)