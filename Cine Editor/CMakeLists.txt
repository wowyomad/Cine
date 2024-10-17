set(CineEditor "CineEditor")
set(CineEditor_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(${CMAKE_SOURCE_DIR}/copy_functions.cmake)

file(GLOB EDITOR_TEXTURES "${CineEditor_SOURCE_DIR}/Assets/Textures/*")
file(GLOB EDITOR_FONTS "${CineEditor_SOURCE_DIR}/Assets/Fonts/JetBrainsMono/ttf/*")
set(EDITOR_TEXTURES_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Textures")
set(EDITOR_FONTS_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Fonts")


message ("EDITOR OUT DIRS ${EDITOR_TEXTURES_OUT} ${EDITOR_FONTS_OUT}")
copy_files("${EDITOR_TEXTURES}" "${EDITOR_TEXTURES_OUT}" editor_copy_textures)
copy_files("${EDITOR_FONTS}" "${EDITOR_FONTS_OUT}" editor_copy_fonts)


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

add_dependencies(${CineEditor} editor_copy_textures editor_copy_fonts)