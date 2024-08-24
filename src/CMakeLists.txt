set(GLASH_SOURCE_DIR "glash")
project(glash_library)
file(GLOB SOURCES
    "${GLASH_SOURCE_DIR}/*.cpp"
    "${GLASH_SOURCE_DIR}/helper/*.cpp"
)

file(GLOB HEADERS
    "${GLASH_SOURCE_DIR}/*.hpp"
    "${GLASH_SOURCE_DIR}/helper/*.hpp"
)

if(GLASH_BUILD_SHARED)
    set(GLASH_PROPERTIES SHARED)
    else()
    set(GLASH_PROPERTIES STATIC)
endif()

add_library(glash_pch STATIC glash_pch.hpp glash_pch.cpp)
target_precompile_headers(glash_pch PRIVATE glash_pch.hpp)
add_library(glash ${GLASH_PROPERTIES} ${SOURCES} ${HEADERS})
target_include_directories(glash_pch
    PUBLIC ${fmt_SOURCE_DIR}/include
    PUBLIC ${glew_SOURCE_DIR}/include
    PUBLIC ${glm_SOURCE_DIR}/include
)
target_link_libraries(glash_pch
    PUBLIC glm
)

set_target_properties(glash PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_link_libraries(glash
    PUBLIC OpenGL::GL
    PUBLIC glfw
    PUBLIC libglew_static
	PUBLIC fmt
    PUBLIC glm
    PRIVATE glash_pch
)

target_include_directories(glash
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${glew_SOURCE_DIR}/include
    PUBLIC ${glfw_SOURCE_DIR}/include
    PUBLIC ${OPENGL_INCLUDE_DIR}
    PUBLIC ${fmt_SOURCE_DIR}/include
    PUBLIC ${glm_SOURCE_DIR}/include
)

set_target_properties(glash PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

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