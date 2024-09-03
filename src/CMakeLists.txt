set(GLASH_SOURCE_DIR "glash")
set(GLASH_PCH_HEADER "${GLASH_SOURCE_DIR}/glash_pch.hpp")


# LOG!!!
remove_definitions(-DNO_LOGGING)

# Gather source and header files
file(GLOB SOURCES
    "${GLASH_SOURCE_DIR}/*.cpp"
    "${GLASH_SOURCE_DIR}/helper/*.cpp"
)

file(GLOB HEADERS
    "${GLASH_SOURCE_DIR}/*.hpp"
    "${GLASH_SOURCE_DIR}/helper/*.hpp"
)

# Set library type (shared or static)
if(GLASH_BUILD_SHARED)
    set(GLASH_PROPERTIES SHARED)
else()
    set(GLASH_PROPERTIES STATIC)
endif()

# Create the glash library
add_library(glash ${GLASH_PROPERTIES} ${SOURCES} ${HEADERS} )

# Set PCH for glash
target_precompile_headers(glash PRIVATE ${GLASH_PCH_HEADER})

# Link dependencies
target_include_directories(glash
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${glew_SOURCE_DIR}/include
    PUBLIC ${glfw_SOURCE_DIR}/include
    PUBLIC ${OPENGL_INCLUDE_DIR}
    PUBLIC ${fmt_SOURCE_DIR}/include
    PUBLIC ${spdlog_SOURCE_DIR}/include
    PUBLIC ${glm_SOURCE_DIR}/include
)

target_link_libraries(glash
    PUBLIC OpenGL::GL
    PUBLIC glfw
    PUBLIC libglew_static
    PUBLIC fmt
    PUBLIC glm
    PUBLIC spdlog::spdlog
)
# For hot reload in msvc
if(MSVC)
    target_compile_options(glash PRIVATE $<$<CONFIG:Debug>:/Zi>)
    target_link_options(glash PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)

endif()

# Set output directories
set_target_properties(glash PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    FOLDER "glash"
)

# Installation setup
install(TARGETS glash
    EXPORT glashTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${GLASH_SOURCE_DIR}/
    DESTINATION include/glash
    FILES_MATCHING PATTERN "*.hpp"
)
