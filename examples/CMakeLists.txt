add_executable(example1 example1.cpp)

target_link_libraries(example1 PRIVATE glash)


set_target_properties(example1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)