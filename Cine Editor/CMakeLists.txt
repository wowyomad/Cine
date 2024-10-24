set(CineEditor "CineEditor")
set(CineEditor_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(${GLASH_ROOT_DIR}/copy_functions.cmake)


file(GLOB EDITOR_FONTS "${CineEditor_SOURCE_DIR}/Resources/Fonts/JetBrainsMono/ttf/*")
file(GLOB EDITOR_CONTENTBROWSER_ICONS "${CineEditor_SOURCE_DIR}/Resources/Icons/ContentBrowser/*")
file(GLOB EDITOR_TEXTURES "${CineEditor_SOURCE_DIR}/Assets/Textures/*")
file(GLOB EDITOR_SHADERS "${CineEditor_SOURCE_DIR}/Resources/Shaders/*")

set(EDITOR_FONTS_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources/Fonts")
set(EDITOR_CONTENTBROWSER_ICONS_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources/Icons/ContentBrowser")
set(EDITOR_TEXTURES_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Textures")
set(EDITOR_SHADERS_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources/Shaders")

set(PLUGIN_SOURCE_DIR "${CineEditor_SOURCE_DIR}/Assets/Plugin")
set(SCRIPTS_SOURCE_DIR "${CineEditor_SOURCE_DIR}/Assets/Scripts")
set(DEST_PLUGIN_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Plugin")
set(DEST_SCRIPTS_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Scripts")

add_custom_target(copy_plugin ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PLUGIN_SOURCE_DIR} ${DEST_PLUGIN_PATH}
    COMMENT "Copying Plugin directory to ${DEST_PLUGIN_PATH}"
)
add_custom_target(copy_scripts ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SCRIPTS_SOURCE_DIR} ${DEST_SCRIPTS_PATH}
    COMMENT "Copying Scripts directory to ${DEST_SCRIPTS_PATH}"
)

copy_files("${EDITOR_TEXTURES}" "${EDITOR_TEXTURES_OUT}" editor_copy_textures)
copy_files("${EDITOR_FONTS}" "${EDITOR_FONTS_OUT}" editor_copy_fonts)
copy_files("${EDITOR_CONTENTBROWSER_ICONS}" "${EDITOR_CONTENTBROWSER_ICONS_OUT}" editor_copy_contentbrowser_icons)
copy_files("${EDITOR_SHADERS}" "${EDITOR_SHADERS_OUT}" editor_copy_shaders)

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

add_dependencies(${CineEditor} editor_copy_textures editor_copy_fonts editor_copy_contentbrowser_icons editor_copy_shaders copy_plugin copy_scripts)