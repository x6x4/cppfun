
SET(LIB_NAME "instr_set")

add_library (${LIB_NAME} SHARED ${LIB_NAME}.cpp ${LIB_NAME}.h)
target_link_libraries(instr_set cpu)
