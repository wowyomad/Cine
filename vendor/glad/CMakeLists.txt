set(glad_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(glad_INCLUDE_DIR ${glad_SOURCE_DIR}/include)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC ${glad_INCLUDE_DIR})

set_target_properties(glad PROPERTIES 
	FOLDER "Dependencies"
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
