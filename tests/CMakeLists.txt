include(${CMAKE_SOURCE_DIR}/copy_functions.cmake)

# Set directories
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(SHADERS_DIR ${RESOURCES_DIR}/shaders)
set(TEXTURES_DIR ${RESOURCES_DIR}/textures)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(RESOURCES_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
set(SHADERS_OUT_DIR ${RESOURCES_OUT_DIR}/shaders)
set(TEXTURES_OUT_DIR ${RESOURCES_OUT_DIR}/textures)

file(GLOB SHADER_FILES "${SHADERS_DIR}/*.frag" "${SHADERS_DIR}/*.vert" "${SHADERS_DIR}/*.shader")
file(GLOB TEXTURE_FILES "${TEXTURES_DIR}/*.png" "${TEXTURES_DIR}/*.jpg")

copy_files("${SHADER_FILES}" "${SHADERS_OUT_DIR}" copy_shaders)
copy_files("${TEXTURE_FILES}" "${TEXTURES_OUT_DIR}" copy_textures)

add_executable(test_runner 
test_main.cpp 
TestClearColor.cpp
TestMenu.cpp
TestTexture.cpp
)

set_target_properties(test_runner PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(test_runner glash)
add_dependencies(test_runner copy_shaders copy_textures)

if(MSVC)
    target_compile_options(test_runner PRIVATE $<$<CONFIG:Debug>:/Zi>)
    target_link_options(test_runner PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)

endif()


