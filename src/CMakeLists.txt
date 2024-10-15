set(glash_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glash" CACHE STRING "" FORCE)
set(GLASH_PCH_HEADER "${glash_SOURCE_DIR}/glash_pch.hpp" CACHE STRING "" FORCE)
set(GLASH_PCH_HEADER_CPP "${glash_SOURCE_DIR}/glash_pch.cpp" CACHE STRING "" FORCE)
find_package(OpenGL REQUIRED)

file(GLOB SOURCES
    "${glash_SOURCE_DIR}/*.cpp"
    "${VENDOR_DIR}/stb/stb_image.cpp"
    "${glash_SOURCE_DIR}/helper/*.cpp"
    "${glash_SOURCE_DIR}/events/*.cpp"
    "${glash_SOURCE_DIR}/vendor/*.cpp"
    "${glash_SOURCE_DIR}/utils/*.cpp"
    "${glash_SOURCE_DIR}/platform/Windows/*.cpp"
    "${glash_SOURCE_DIR}/platform/OpenGL/*.cpp"
    "${glash_SOURCE_DIR}/ImGui/*.cpp"
    "${glash_SOURCE_DIR}/Core/*.cpp"
    "${glash_SOURCE_DIR}/Scene/*.cpp"
    "${glash_SOURCE_DIR}/Renderer/*.cpp"
)

file(GLOB HEADERS
    "${glash_SOURCE_DIR}/*.hpp"
    "${glash_SOURCE_DIR}/helper/*.hpp"
    "${glash_SOURCE_DIR}/events/*.hpp"
    "${glash_SOURCE_DIR}/utils/*.hpp"
    "${glash_SOURCE_DIR}/platform/Windows/*.hpp"
    "${glash_SOURCE_DIR}/platform/OpenGL/*.hpp"
    "${glash_SOURCE_DIR}/platform/Platforms.hpp"
    "${glash_SOURCE_DIR}/ImGui/*.hpp"
    "${glash_SOURCE_DIR}/Core/*.hpp"
    "${glash_SOURCE_DIR}/Renderer/*.hpp"
    "${glash_SOURCE_DIR}/Scene/*.hpp"
    "${glash_SOURCE_DIR}/Debug/*.hpp"
)
if(GLASH_ENABLE_DEBUG)
    list(APPEND GLASH_COMPILE_DEFINITIONS GLASH_ENABLE_DEBUG)
    if(CINE_ENABLE_ASSERTS)
        list(APPEND GLASH_COMPILE_DEFINITIONS CINE_ENABLE_ASSERTS)
    endif()
endif()

if(GLASH_BUILD_DLL)
    set(GLASH_LIB_TYPE SHARED)
    list(APPEND GLASH_COMPILE_DEFINITIONS GLASH_BUILD_DLL GLASH_EXPORT)
else()
    set(GLASH_LIB_TYPE STATIC)
endif()
message("Glash build type: ${GLASH_LIB_TYPE}")
message("Glash compile definitions: ${GLASH_COMPILE_DEFINITIONS}")

add_library(glash_pch STATIC ${GLASH_PCH_HEADER_CPP} )
target_include_directories(glash_pch
PUBLIC ${glash_SOURCE_DIR}
PUBLIC ${glad_SOURCE_DIR}/include
PUBLIC ${fmt_SOURCE_DIR}/include
PUBLIC ${glm_SOURCE_DIR}
)
target_precompile_headers(glash_pch PUBLIC ${GLASH_PCH_HEADER})

add_library(glash ${GLASH_LIB_TYPE} ${SOURCES} ${HEADERS} )
target_compile_definitions(glash PRIVATE ${GLASH_COMPILE_DEFINITIONS})

target_precompile_headers(glash REUSE_FROM glash_pch)

target_include_directories(glash
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${glad_SOURCE_DIR}/include
    PUBLIC ${glfw_SOURCE_DIR}/include
    PUBLIC ${OPENGL_INCLUDE_DIR}
    PUBLIC ${fmt_SOURCE_DIR}/include
    PUBLIC ${spdlog_SOURCE_DIR}/include
    PUBLIC ${glm_SOURCE_DIR}
    PUBLIC ${stb_SOURCE_DIR}
    PUBLIC ${imgui_SOURCE_DIR}
    PUBLIC ${EnTT_SOURCE_DIR}
)

target_link_libraries(glash
    PRIVATE OpenGL::GL
    PUBLIC glfw
    PUBLIC glad
    PUBLIC fmt
    PUBLIC glm
    PUBLIC spdlog::spdlog
    PUBLIC imgui
    PUBLIC EnTT::EnTT 
    PUBLIC yaml-cpp::yaml-cpp
)

if(MSVC)
    target_compile_options(glash PRIVATE /MP)
    target_compile_options(glash PRIVATE $<$<CONFIG:Debug>:/Zi>)
    target_link_options(glash PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
endif()

set_target_properties(glash PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    FOLDER "glash"
)
set_target_properties(glash_pch PROPERTIES FOLDER "glash")

