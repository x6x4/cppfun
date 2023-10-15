
add_subdirectory(qasm)
add_subdirectory(mem)
add_subdirectory(cpu)

target_link_libraries(cpu mcode_compiler mem)
