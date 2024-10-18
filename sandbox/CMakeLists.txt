include(${CMAKE_SOURCE_DIR}/copy_functions.cmake)

file(GLOB SANDBOX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/*.cpp"
)
file(GLOB SANDBOX_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/*.hpp"
)

# Set directories
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
set(SHADERS_DIR ${RESOURCES_DIR}/Shaders)
set(TEXTURES_DIR ${RESOURCES_DIR}/Textures)

# Output directories
set(RESOURCES_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets)
set(SHADERS_OUT_DIR ${RESOURCES_OUT_DIR}/Shaders)
set(TEXTURES_OUT_DIR ${RESOURCES_OUT_DIR}/Textures)

file(GLOB SHADER_FILES "${SHADERS_DIR}/*.frag" "${SHADERS_DIR}/*.vert" "${SHADERS_DIR}/*.shader" "${SHADERS_DIR}/*.glsl")
file(GLOB TEXTURE_FILES "${TEXTURES_DIR}/*.png" "${TEXTURES_DIR}/*.jpg" "${TEXTURES_DIR}/*.bmp")

copy_files("${SHADER_FILES}" "${SHADERS_OUT_DIR}" copy_shaders)
copy_files("${TEXTURE_FILES}" "${TEXTURES_OUT_DIR}" copy_textures)

add_executable(Sandbox ${SANDBOX_SOURCES} ${SANDBOX_HEADERS})
if(GLASH_ENABLE_DEBUG)
    list(APPEND SANDBOX_COMPILE_DEFINITIONS GLASH_ENABLE_DEBUG)
endif()

target_compile_definitions(Sandbox PRIVATE ${SANDBOX_COMPILE_DEFINITIONS})

target_link_libraries(Sandbox
    PRIVATE glash 
)
add_dependencies(Sandbox copy_shaders copy_textures)
target_precompile_headers(Sandbox REUSE_FROM glash_pch)

if(MSVC)
    target_compile_options(Sandbox PRIVATE $<$<CONFIG:Debug>:/Zi>) #ignore definitions that came from precompiled header
    target_link_options(Sandbox PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
endif()

set_target_properties(Sandbox PROPERTIES
    FOLDER "glash"
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
set_target_properties(copy_textures copy_shaders PROPERTIES FOLDER "glash/Sandbox Commands")
