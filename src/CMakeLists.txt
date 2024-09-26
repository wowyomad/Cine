set(glash_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glash")
set(GLASH_PCH_HEADER "${glash_SOURCE_DIR}/glash_pch.hpp")

if(TARGET libglew_static)
    set_target_properties(libglew_static PROPERTIES FOLDER "Dependencies")
else()
    message("Static GLEW target is not available")
endif()
if(TARGET glfw)
    set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
else()
    message("GLFW target is not available")
endif()
if(TARGET fmt)
    set_target_properties(fmt PROPERTIES FOLDER "Dependencies")
else()
    message("FMT target is not available")
endif()

if(TARGET glm)
    set_target_properties(glm PROPERTIES FOLDER "Dependencies")
else()
    message("GLM target is not available")
endif()
if(TARGET spdlog)
    set_target_properties(spdlog PROPERTIES FOLDER "Dependencies")
else()
    message("spdlog target is not available")
endif()

find_package(OpenGL REQUIRED)

file(GLOB SOURCES
    "${glash_SOURCE_DIR}/*.cpp"
    "${glash_SOURCE_DIR}/helper/*.cpp"
    "${glash_SOURCE_DIR}/events/*.cpp"
    "${glash_SOURCE_DIR}/vendor/*.cpp"
    "${glash_SOURCE_DIR}/utils/*.cpp"
)

file(GLOB HEADERS
    "${glash_SOURCE_DIR}/*.hpp"
    "${glash_SOURCE_DIR}/helper/*.hpp"
    "${glash_SOURCE_DIR}/events/*.hpp"
    "${glash_SOURCE_DIR}/vendor/*.hpp"
    "${glash_SOURCE_DIR}/utils/*.hpp"

)

if(GLASH_BUILD_DLL)
    set(GLASH_LIB_TYPE SHARED)
    set(GLASH_COMPILE_DEFINITIONS GLASH_BUILD_DLL GLASH_EXPORT)
else()
    set(GLASH_LIB_TYPE STATIC)
    set(GLASH_COMPILE_DEFINITIONS "")
endif()
message("Glash build type: ${GLASH_LIB_TYPE}")
message("Glash compile definitions: ${GLASH_COMPILE_DEFINITIONS}")

add_library(glash ${GLASH_LIB_TYPE} ${SOURCES} ${HEADERS})
target_compile_definitions(glash PRIVATE ${GLASH_COMPILE_DEFINITIONS})

target_precompile_headers(glash PUBLIC ${GLASH_PCH_HEADER})

target_include_directories(glash
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${glew_SOURCE_DIR}/include
    PUBLIC ${glfw_SOURCE_DIR}/include
    PUBLIC ${OPENGL_INCLUDE_DIR}
    PUBLIC ${fmt_SOURCE_DIR}/include
    PUBLIC ${spdlog_SOURCE_DIR}/include
    PUBLIC ${glm_SOURCE_DIR}/include
    PUBLIC ${stb_SOURCE_DIR}
)

target_link_libraries(glash
    PRIVATE OpenGL::GL
    PUBLIC glfw
    PUBLIC libglew_static
    PUBLIC fmt
    PUBLIC glm
    PUBLIC spdlog::spdlog
)

if(MSVC)
    target_compile_options(glash PRIVATE $<$<CONFIG:Debug>:/Zi>)
    target_link_options(glash PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)

endif()

set_target_properties(glash PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    FOLDER "glash"
)