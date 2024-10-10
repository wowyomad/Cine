include(${CMAKE_SOURCE_DIR}/copy_functions.cmake)

set(GAME_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(GAME_ASSETS_TEXTURES_DIR "${GAME_ASSETS_DIR}/textures")
set(GAME_ASSETS_SHADERS_DIR "${GAME_ASSETS_DIR}/shaders")

set(GAME_ASSETS_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

# Call the function with multiple directories and a single target
copy_files_X("${GAME_ASSETS_OUT_DIR}" copy_game_assets "${GAME_ASSETS_TEXTURES_DIR}" "${GAME_ASSETS_SHADERS_DIR}")

file(GLOB game_SOURCE "src/Game.cpp")

add_executable(game ${game_SOURCE})
target_include_directories(game
PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src"
PUBLIC "${glash_SOURCE_DIR}"
)
target_link_libraries(game glash)
target_precompile_headers(game REUSE_FROM glash_pch)
add_dependencies(game copy_game_assets)

if(MSVC)
    target_compile_options(game PRIVATE $<$<CONFIG:Debug>:/Zi>)
    target_link_options(game PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
endif()