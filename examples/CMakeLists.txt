cmake_minimum_required(VERSION 3.10)
project(TimingExample)

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
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
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

file(GLOB SHADER_FILES
    "${SHADERS_DIR}/*.frag"
    "${SHADERS_DIR}/*.vert"
    "${SHADERS_DIR}/*.shader"
)

file(GLOB IMAGE_FILES
    "${TEXTURES_DIR}/*.png"
    "${TEXTURES_DIR}/*.jpg"
)

# Create the shaders output directory
add_custom_command(
    OUTPUT ${SHADERS_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_OUT_DIR}
    COMMENT "Creating shaders output directory"
)
# Create the images output directory
add_custom_command(
    OUTPUT ${TEXTURES_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TEXTURES_OUT_DIR}
    COMMENT "Creating images output directory"
)

# Copy shader files
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
# Copy image files
foreach(IMAGE_FILE ${IMAGE_FILES})
    get_filename_component(IMAGE_NAME ${IMAGE_FILE} NAME)
    set(DEST_FILE ${TEXTURES_OUT_DIR}/${IMAGE_NAME})
    
    add_custom_command(
        OUTPUT ${DEST_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${IMAGE_FILE} ${DEST_FILE}
        COMMAND ${CMAKE_COMMAND} -E echo "Copied ${IMAGE_NAME} to output directory"
        COMMENT "Checking if ${IMAGE_NAME} needs to be copied"
        DEPENDS ${IMAGE_FILE}
    )
    
    list(APPEND COPIED_TEXTURES ${DEST_FILE})
endforeach()

# Create custom targets for shaders and images
add_custom_target(copy_shaders ALL DEPENDS ${COPIED_SHADERS})
add_custom_target(copy_images ALL DEPENDS ${COPIED_TEXTURES})

# Clean up shaders and images directories
add_custom_target(clean_shaders
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${SHADERS_OUT_DIR}
    COMMENT "Removing shaders directory and its contents"
)
add_custom_target(clean_images
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TEXTURES_OUT_DIR}
    COMMENT "Removing images directory and its contents"
)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${SHADERS_OUT_DIR})
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${TEXTURES_OUT_DIR})

add_executable(example1 "glash_example.cpp")
add_dependencies(example1 copy_shaders copy_images)

add_executable(example2 "imgui_example.cpp")
add_dependencies(example2 copy_shaders copy_images)

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

set_target_properties(copy_shaders clean_shaders copy_images clean_images PROPERTIES
    FOLDER "Examples"
)
