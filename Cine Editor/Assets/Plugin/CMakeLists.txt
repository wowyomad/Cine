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
	GIT_TAG 129e8aff065e65308ab3e836720ffc2bbea0b644
)
FetchContent_MakeAvailable(glash_lib)

add_library(plugin SHARED "Generated/plugin_generated.cpp" plugin.hpp)
target_link_libraries(plugin PUBLIC glash)
message("Source dir of glash : ${glash_SOURCE_DIR}")
target_include_directories(plugin PUBLIC ${glash_SOURCE_DIR})
target_include_directories(plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND PLUGIN_PROPERTIES  
    GLASH_ENABLE_DEBUG)
target_compile_definitions(plugin PRIVATE ${PLUGIN_PROPERTIES})

set_target_properties(plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    FOLDER "Plugin"
)