set(CINE_EDITOR "CineEditor")
set(CineEditor_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES
	${CineEditor_SOURCE_DIR}/CineEditor/*.cpp
)

add_executable(${CINE_EDITOR} ${SOURCES})
target_link_libraries(${CINE_EDITOR} PRIVATE glash)
target_include_directories(${CINE_EDITOR} PRIVATE ${glash_SOURCE_DIR})
target_precompile_headers(${CINE_EDITOR} REUSE_FROM glash_pch)