cmake_minimum_required(VERSION 3.20)
project(Cine VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

include(set_undefined.cmake)
# Project properties
set_undefined(GLASH_ENABLE_DEBUG ON CACHE BOOL "Enable debug logging" FORCE)
set_undefined(GLASH_BUILD_DLL OFF CACHE BOOL "Build Cine as a shared library" FORCE)
set_undefined(GLASH_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set_undefined(GLASH_BUILD_SANDDOX ON CACHE BOOL "Build sandbox" FORCE)
set_undefined(CINE_ENABLE_ASSERTS ON CACHE BOOL "Enable asserts" FORCE)

# Might be needed for planned hot reload.
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE) 

# User folders to hide dependency targets
set_property(GLOBAL PROPERTY USE_FOLDER ON)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use fetched fmt instead of bundled" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Enable wayland scanner" FORCE)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.0.2
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG f75e8d1cad7d90d72ef7a4661f1b994ef78b4e31
)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 793773209bb0fdeb8ccb756052e770457193c9f2
)
list(APPEND BUILD_DEPENDENCIES glfw fmt glm spdlog imgui)
list(APPEND HEADER_DEPENDENCIES stb)
list(APPEND DEPENDENCY_TARGETS ${BUILD_DEPENDENCIES} ${HEADER_DEPENDENCIES})
FetchContent_MakeAvailable(${DEPENDENCY_TARGETS})

# Use vendor OpenGL 
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

set(glad_SOURCE_DIR ${VENDOR_DIR}/glad)

add_subdirectory(src)
add_subdirectory(vendor)
if (GLASH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if (GLASH_BUILD_SANDDOX)
    add_subdirectory(sandbox)
endif()

if(MSVC)
    message("Adding compile options for MSVC")
    foreach(target ${BUILD_DEPENDENCIES})
    if(TARGET ${target})
        target_compile_options(${target} PRIVATE /MP)
 #       target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>:/Zi>)
 #       target_link_options(${target} PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
        set_target_properties(${target} PROPERTIES FOLDER "Dependencies")

    else()
        message(WARNING "Target ${target} not found.")
    endif()
endforeach()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message("Adding compile options for GNU")

    target_compile_options(glash PRIVATE -pthread) 
     if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-Werror)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-Wno-unused-parameter)
    endif()
endif()

