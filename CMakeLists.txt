cmake_minimum_required(VERSION 3.20)
project(glash VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use vendor OpenGL 
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.2.0
)
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
FetchContent_MakeAvailable(glew glfw fmt glm)

set(GLASH_BUILD_SHARED OFF CACHE BOOL "Build glash as a shared library"  FORCE)
set(GLASH_BUILD_EXAMPLES ON CACHE BOOL "Build examples"  FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE) 

# Build GLFW and GLEW statically
set(GLFW_BUILD_SHARED OFF CACHE BOOL "Build GLFW as a static library" FORCE)
set(GLEW_BUILD_SHARED OFF CACHE BOOL "Build GLEW as a static library" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support" FORCE)


if(GLEW_BUILD_SHARED)
    set(glew-cmake_BUILD_SHARED ON)
    set(glew-cmake_BUILD_STATIC OFF)
else()
    set(glew-cmake_BUILD_SHARED OFF)
    set(glew-cmake_BUILD_STATIC ON)
endif()


find_package(OpenGL REQUIRED)

add_subdirectory(src)
if (GLASH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -pedantic)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-Werror)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-Wno-unused-parameter)
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/MP)

    add_compile_options(
        $<$<CONFIG:Debug>:/W4 /WX /ZI>      # Debug configuration: high warning level, treat warnings as errors, enable edit and continue
        $<$<CONFIG:Release>:/W3 /O2>        # Release configuration: moderate warning level, enable optimizations
        $<$<CONFIG:RelWithDebInfo>:/W3 /O2> # RelWithDebInfo: same as Release
    )
endif()