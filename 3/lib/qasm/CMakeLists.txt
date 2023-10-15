
add_subdirectory(cmd_base)
add_subdirectory(mcode_compiler)

target_link_libraries(mcode_compiler cmd_base)

