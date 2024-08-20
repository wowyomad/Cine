add_executable(example1 example1.cpp)
add_executable(example2 example2.cpp)


target_link_libraries(example1 PRIVATE glash)
target_link_libraries(example2 PRIVATE glash)


set_target_properties(example1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(example2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)