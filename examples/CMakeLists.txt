cmake_minimum_required(VERSION 3.10)
project(Examples)

include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.2
)

FetchContent_MakeAvailable(imgui)
message("Imgui dir: ${imgui_SOURCE_DIR}")
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_include_directories(imgui PUBLIC ${glfw_SOURCE_DIR}/include)

# Set directories
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(SHADERS_DIR ${RESOURCES_DIR}/shaders)
set(TEXTURES_DIR ${RESOURCES_DIR}/textures)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(RESOURCES_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/resources)
set(SHADERS_OUT_DIR ${RESOURCES_OUT_DIR}/shaders)
set(TEXTURES_OUT_DIR ${RESOURCES_OUT_DIR}/textures)

file(GLOB SHADER_FILES "${SHADERS_DIR}/*.frag" "${SHADERS_DIR}/*.vert" "${SHADERS_DIR}/*.shader")
file(GLOB TEXTURE_FILES "${TEXTURES_DIR}/*.png" "${TEXTURES_DIR}/*.jpg")

# Utility function for shader/image copying
function(copy_files FILE_LIST DEST_DIR TARGET_NAME)
    foreach(FILE ${FILE_LIST})
        get_filename_component(NAME ${FILE} NAME)
        set(DEST_FILE ${DEST_DIR}/${NAME})
        add_custom_command(
            OUTPUT ${DEST_FILE}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${DEST_FILE}
            COMMENT "Copying ${NAME} to ${DEST_DIR}"
            DEPENDS ${FILE}
        )
        list(APPEND COPY_TARGETS ${DEST_FILE})
    endforeach()
    add_custom_target(${TARGET_NAME} ALL DEPENDS ${COPY_TARGETS})
endfunction()

# Copy shaders and textures
copy_files("${SHADER_FILES}" "${SHADERS_OUT_DIR}" copy_shaders)
copy_files("${TEXTURE_FILES}" "${TEXTURES_OUT_DIR}" copy_textures)

add_executable(example1 "glash_example.cpp")
add_dependencies(example1 copy_shaders copy_textures)

add_executable(example2 "imgui_example.cpp")
add_dependencies(example2 copy_shaders copy_textures)

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
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
    FOLDER "Examples"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>"
)

set_target_properties(copy_shaders copy_textures PROPERTIES
    FOLDER "Examples"
)
