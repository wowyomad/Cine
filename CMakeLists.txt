cmake_minimum_required(VERSION 3.20)
project(Cine VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(GLASH_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})


include(set_undefined.cmake)
# Project properties
set_undefined(GLASH_ENABLE_DEBUG ON CACHE BOOL "Enable debug logging" FORCE)
set_undefined(GLASH_BUILD_DLL OFF CACHE BOOL "Build Cine as a shared library" FORCE)
set_undefined(GLASH_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set_undefined(GLASH_BUILD_SANDDOX OFF CACHE BOOL "Build sandbox" FORCE)
set_undefined(GLASH_BUILD_EDITOR ON CACHE BOOL "Build editor" FORCE)
set_undefined(GLASH_ENABLE_ASSERTS ON CACHE BOOL "Enable asserts" FORCE)

# Might be needed for planned hot reload.
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE) 

# User folders to hide dependency targets
set_property(GLOBAL PROPERTY USE_FOLDER ON)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use fetched fmt instead of bundled" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Enable wayland scanner" FORCE)

set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Cames from yaml-cpp. Builds 'read' and 'parse' executables" FORCE)

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
	GIT_PROGRESS TRUE
)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.0.2
	GIT_PROGRESS TRUE
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
	GIT_PROGRESS TRUE
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
	GIT_PROGRESS TRUE
)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG f75e8d1cad7d90d72ef7a4661f1b994ef78b4e31
	GIT_PROGRESS TRUE
)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 793773209bb0fdeb8ccb756052e770457193c9f2
	GIT_PROGRESS TRUE
)
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.13.2
	GIT_PROGRESS TRUE
)
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0
	GIT_PROGRESS TRUE
)
FetchContent_Declare(
    ImGuizmo
    GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo.git
    GIT_TAG 6d588209f99b1324a608783d1f52faa518886c29 # 24.09.2024
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(ImGuizmo) #It's special

list(APPEND BUILD_DEPENDENCIES glfw fmt glm spdlog imgui entt yaml-cpp)
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
if (GLASH_BUILD_EDITOR)
    add_subdirectory("Cine Editor")
endif()
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

