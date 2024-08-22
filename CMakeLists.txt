cmake_minimum_required(VERSION 3.20)
project(glash VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /ZI")
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
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/W4 /WX)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/W3)
    endif()
endif()
