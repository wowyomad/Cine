cmake_minimum_required(VERSION 3.10)
project(Examples)

include(${CMAKE_SOURCE_DIR}/copy_functions.cmake)

# Set directories
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(SHADERS_DIR ${RESOURCES_DIR}/shaders)
set(TEXTURES_DIR ${RESOURCES_DIR}/textures)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(RESOURCES_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/resources)
set(SHADERS_OUT_DIR ${RESOURCES_OUT_DIR}/shaders)
set(TEXTURES_OUT_DIR ${RESOURCES_OUT_DIR}/textures)

file(GLOB SHADER_FILES "${SHADERS_DIR}/*.frag" "${SHADERS_DIR}/*.vert" "${SHADERS_DIR}/*.shader")
file(GLOB TEXTURE_FILES "${TEXTURES_DIR}/*.png" "${TEXTURES_DIR}/*.jpg")

# Copy shaders and textures
copy_files("${SHADER_FILES}" "${SHADERS_OUT_DIR}" copy_shaders_examples)
copy_files("${TEXTURE_FILES}" "${TEXTURES_OUT_DIR}" copy_textures_examples)

add_executable(example1 "glash_example.cpp")
add_dependencies(example1 copy_shaders_examples copy_textures_examples)

add_executable(example2 "imgui_example.cpp")
add_dependencies(example2 copy_shaders_examples copy_textures_examples)

target_link_libraries(example1 PRIVATE glash imgui)
target_link_libraries(example2 PRIVATE glash imgui)

target_include_directories(example1 PRIVATE ${imgui_SOURCE_DIR})
target_include_directories(example2 PRIVATE ${imgui_SOURCE_DIR})

set(GLASH_PCH_HEADER "${CMAKE_SOURCE_DIR}/src/glash/glash_pch.hpp")
target_precompile_headers(example1 PRIVATE ${GLASH_PCH_HEADER})

if(MSVC)
    target_compile_options(example1 PRIVATE $<$<CONFIG:Debug>:/Zi>)
    target_link_options(example1 PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
endif()

set_target_properties(example1 example2 PROPERTIES
    FOLDER "Examples"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

set_target_properties(copy_shaders_examples copy_textures_examples PROPERTIES
    FOLDER "Examples"
)
