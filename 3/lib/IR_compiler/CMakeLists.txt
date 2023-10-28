SET(LIB_NAME "IR_compiler")

add_library(${LIB_NAME} ${LIB_NAME}.cpp)
target_link_libraries(${LIB_NAME} cpu)
