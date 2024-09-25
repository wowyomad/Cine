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

add_executable(Sandbox 
Sandbox.cpp
TestClearColor.cpp
TestMenu.cpp
TestTexture.cpp
ImguiWindow.cpp
)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
 )
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${glfw_SOURCE_DIR}/include)

message("GLASH_BUILD_DLL in Sandbox: ${GLASH_BUILD_DLL}")
if(GLASH_BUILD_DLL) 
    set(SANDBOX_COMPILE_DEFINITIONS GLASH_BUILD_DLL GLASH_IMPORT)
else()
    set(SANDBOX_COMPILE_DEFINITIONS "")
endif()
message("SANDBOX_COMPILE_DEFINITIONS in Sandbox: ${SANDBOX_COMPILE_DEFINITIONS}")

target_compile_definitions(Sandbox PRIVATE ${SANDBOX_COMPILE_DEFINITIONS})
set_target_properties(Sandbox PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(Sandbox
    PRIVATE glash 
    PUBLIC glfw
    PUBLIC libglew_static
    PRIVATE imgui
)
add_dependencies(Sandbox copy_shaders copy_textures)
target_precompile_headers(Sandbox PRIVATE ${GLASH_PCH_HEADER})


if(MSVC)
    target_compile_options(Sandbox PRIVATE $<$<CONFIG:Debug>:/Zi>)
    target_link_options(Sandbox PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)

endif()


