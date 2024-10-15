set(CineEditor "CineEditor")
set(CineEditor_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES
	${CineEditor_SOURCE_DIR}/CineEditor/*
	${CineEditor_SOURCE_DIR}/CineEditor/Panels/*
)

list(APPEND editor_PRECOMPILE_DEFINITIONS GLASH_ENABLE_DEBUG)

add_executable(${CineEditor} ${SOURCES})
target_compile_definitions(${CineEditor} PRIVATE ${editor_PRECOMPILE_DEFINITIONS})
target_link_libraries(${CineEditor} PRIVATE glash)
target_include_directories(${CineEditor}
	PRIVATE ${glash_SOURCE_DIR}
)
target_precompile_headers(${CineEditor} REUSE_FROM glash_pch)