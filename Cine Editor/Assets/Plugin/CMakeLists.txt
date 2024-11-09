cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Plugin)
include(FetchContent)
set(GLASH_BUILD_SANDDOX OFF)
set(GLASH_BUILD_EDITOR OFF)

FetchContent_Declare(
	glash_lib
	GIT_REPOSITORY https://github.com/wowyomad/cine.git
	GIT_TAG 79aa6c91d0ef2549216b82df2c30684ab08f25d3
)
FetchContent_MakeAvailable(glash_lib)

add_library(plugin SHARED "Generated/plugin_generated.cpp" "input.cpp")
target_link_libraries(plugin PUBLIC glash)
message("Source dir of glash : ${glash_SOURCE_DIR}")
target_include_directories(plugin PUBLIC ${glash_SOURCE_DIR})
target_include_directories(plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND PLUGIN_PROPERTIES  
    GLASH_ENABLE_DEBUG)
target_compile_definitions(plugin PRIVATE ${PLUGIN_PROPERTIES})
target_precompile_headers(plugin PRIVATE "scripts_pch.hpp")

set_target_properties(plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    FOLDER "Plugin"
)

if(MSVC)
 # target_link_options(plugin PRIVATE "/DEBUG:NONE")
endif()

if(MSVC)
    add_custom_command(TARGET plugin PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Removing existing .pdb file if it exists"
        COMMAND ${CMAKE_COMMAND} -E echo "File path: ${CMAKE_CURRENT_SOURCE_DIR}/lib/Debug/plugin.pdb"
        COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_CURRENT_SOURCE_DIR}/lib/Debug/plugin.pdb"
        COMMENT "Removing .pdb file before build"
        VERBATIM
    )
endif()

