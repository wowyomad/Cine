set(GLASH_SOURCE_DIR "glash")
set(GLASH_PCH_HEADER "${GLASH_SOURCE_DIR}/glash_pch.hpp")

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
    message("GLM target is not available")
endif()

find_package(OpenGL REQUIRED)



file(GLOB SOURCES
    "${GLASH_SOURCE_DIR}/*.cpp"
    "${GLASH_SOURCE_DIR}/helper/*.cpp"
    "${GLASH_SOURCE_DIR}/vendor/*.cpp"
)

file(GLOB HEADERS
    "${GLASH_SOURCE_DIR}/*.hpp"
    "${GLASH_SOURCE_DIR}/helper/*.hpp"
    "${GLASH_SOURCE_DIR}/vendor/*.hpp"
)

if(GLASH_BUILD_SHARED)
    set(GLASH_PROPERTIES SHARED)
else()
    set(GLASH_PROPERTIES STATIC)
endif()

add_library(glash ${GLASH_PROPERTIES} ${SOURCES} ${HEADERS})

target_precompile_headers(glash PRIVATE ${GLASH_PCH_HEADER})

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
    PUBLIC OpenGL::GL
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
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    FOLDER "glash"
)