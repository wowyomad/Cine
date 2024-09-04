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
    "${SHADERS_DIR}/*.shader"
)

# Create the shaders output directory
add_custom_command(
    OUTPUT ${SHADERS_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_OUT_DIR}
    COMMENT "Creating shaders output directory"
)

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    set(DEST_FILE ${SHADERS_OUT_DIR}/${SHADER_NAME})
    
    add_custom_command(
        OUTPUT ${DEST_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER_FILE} ${DEST_FILE}
        COMMAND ${CMAKE_COMMAND} -E echo "Copied ${SHADER_NAME} to output directory"
        COMMENT "Checking if ${SHADER_NAME} needs to be copied"
        DEPENDS ${SHADER_FILE}
    )
    
    list(APPEND COPIED_SHADERS ${DEST_FILE})
endforeach()

add_custom_target(copy_shaders ALL DEPENDS ${COPIED_SHADERS})

add_custom_target(clean_shaders
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${SHADERS_OUT_DIR}
    COMMENT "Removing shaders directory and its contents"
)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${SHADERS_OUT_DIR})

# Define PCH header path (same as glash library)
set(GLASH_PCH_HEADER "${CMAKE_SOURCE_DIR}/src/glash/glash_pch.hpp")

# Create the example1 executable
add_executable(example1 example1.cpp test_window.hpp)
add_dependencies(example1 copy_shaders)

# Link against the glash library
target_link_libraries(example1 PRIVATE glash)

# Set up the same PCH for example1 as used in glash
target_precompile_headers(example1 PRIVATE ${GLASH_PCH_HEADER})


if(MSVC)
    target_compile_options(example1 PRIVATE $<$<CONFIG:Debug>:/Zi>)
    target_link_options(example1 PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)

endif()

set_target_properties(example1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
    FOLDER "Examples"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>"
)

set_target_properties(copy_shaders clean_shaders PROPERTIES
    FOLDER "Examples"
)
