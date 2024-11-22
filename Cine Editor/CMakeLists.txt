set(CineEditor "CineEditor")
set(CineEditor_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(${GLASH_ROOT_DIR}/copy_functions.cmake)

set(RESOURCES_DIR "${CineEditor_SOURCE_DIR}/Resources")
set(DEST_RESOURCES_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources")

set(ASSETS_DIR "${CineEditor_SOURCE_DIR}/Assets")
set(DEST_ASSETS_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets")

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${RESOURCES_DIR} ${DEST_RESOURCES_DIR}
    COMMENT "Copying Assets directory to ${DEST_RESOURCES_DIR}"
)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${ASSETS_DIR} ${DEST_ASSETS_DIR}
    COMMENT "Copying Resources directory to ${DEST_ASSETS_DIR}"
)

add_custom_target(copy_imgui_ini ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Copying Editor imgui.ini file to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

copy_files("${EDITOR_TEXTURES}" "${EDITOR_TEXTURES_OUT}" editor_copy_textures)
copy_files("${EDITOR_FONTS}" "${EDITOR_FONTS_OUT}" editor_copy_fonts)
copy_files("${EDITOR_CONTENTBROWSER_ICONS}" "${EDITOR_CONTENTBROWSER_ICONS_OUT}" editor_copy_contentbrowser_icons)
copy_files("${EDITOR_SHADERS}" "${EDITOR_SHADERS_OUT}" editor_copy_shaders)

file(GLOB SOURCES
	${CineEditor_SOURCE_DIR}/CineEditor/*
	${CineEditor_SOURCE_DIR}/CineEditor/Panels/*
    ${CineEditor_SOURCE_DIR}/CineEditor/Utils/*
)

list(APPEND editor_PRECOMPILE_DEFINITIONS GLASH_ENABLE_DEBUG)

add_executable(${CineEditor} ${SOURCES})
target_compile_definitions(${CineEditor} PRIVATE ${editor_PRECOMPILE_DEFINITIONS})
target_link_libraries(${CineEditor} PRIVATE glash)
target_include_directories(${CineEditor}
	PRIVATE ${glash_SOURCE_DIR}
)
target_precompile_headers(${CineEditor} REUSE_FROM glash_pch)

add_dependencies(${CineEditor} copy_assets copy_resources copy_imgui_ini)
set_property(TARGET ${CineEditor} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})