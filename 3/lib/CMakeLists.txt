
add_subdirectory(IR)
add_subdirectory(mem)
add_subdirectory(cpu)

SET(LIB_NAME "IR_compiler")

add_library(${LIB_NAME} ${LIB_NAME}.cpp)
target_link_libraries(${LIB_NAME} cpu)
