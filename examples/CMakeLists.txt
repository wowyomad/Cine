cmake_minimum_required(VERSION 3.10)
project(TimingExample)

# Set the shaders source directory
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

# Configure the output directory for the executable and shaders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Use CMAKE_CFG_INTDIR to handle configuration-specific output directories
set(SHADERS_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/shaders)

# Gather all shader files
file(GLOB SHADER_FILES
    "${SHADERS_DIR}/*.frag"
    "${SHADERS_DIR}/*.vert"
)

# Create the shaders output directory
add_custom_command(
    OUTPUT ${SHADERS_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_OUT_DIR}
    COMMENT "Creating shaders output directory"
)

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    add_custom_command(
        OUTPUT ${SHADERS_OUT_DIR}/${SHADER_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER_FILE} ${SHADERS_OUT_DIR}/${SHADER_NAME}
        DEPENDS ${SHADER_FILE}
        COMMENT "Copying ${SHADER_NAME} to output directory"
    )
    list(APPEND COPIED_SHADERS ${SHADERS_OUT_DIR}/${SHADER_NAME})
endforeach()

add_custom_target(copy_shaders ALL DEPENDS ${COPIED_SHADERS})

add_custom_target(clean_shaders
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${SHADERS_OUT_DIR}
    COMMENT "Removing shaders directory and its contents"
)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${SHADERS_OUT_DIR})

add_library(examples_pch STATIC examples_pch.cpp examples_pch.hpp)
target_precompile_headers(examples_pch PRIVATE examples_pch.hpp)

add_executable(example1 example1.cpp)
add_dependencies(example1 copy_shaders)
target_link_libraries(example1 PRIVATE examples_pch glash)

set_target_properties(example1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
)
