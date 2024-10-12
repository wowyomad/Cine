# Utility function for file copying
function(copy_files FILE_LIST DEST_DIR TARGET_NAME)
    foreach(FILE ${FILE_LIST})
        get_filename_component(NAME ${FILE} NAME)
        set(DEST_FILE ${DEST_DIR}/${NAME})
        add_custom_command(
            OUTPUT ${DEST_FILE}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${DEST_FILE}
            COMMENT "Copying ${NAME} to ${DEST_DIR}"
            DEPENDS ${FILE}
        )
        list(APPEND COPY_TARGETS ${DEST_FILE})
    endforeach()
    add_custom_target(${TARGET_NAME} ALL DEPENDS ${COPY_TARGETS})
endfunction()

function(add_copy_target TARGET_NAME DEST_DIR)
    set(SRC_DIRS ${ARGN})
    set(COPY_TARGETS)

    # Determine the configuration directory based on the generator
    if(CMAKE_CONFIGURATION_TYPES)
        # Multi-config generator (e.g., Visual Studio)
        set(CONFIG_DIR "${CMAKE_CFG_INTDIR}")
    else()
        # Single-config generator (e.g., Ninja)
        set(CONFIG_DIR "${CMAKE_BUILD_TYPE}")
    endif()

    # Replace any instance of $<CONFIG> in DEST_DIR with the determined config directory
    string(REPLACE "$<CONFIG>" "${CONFIG_DIR}" DEST_DIR ${DEST_DIR})

    foreach(SRC_DIR ${SRC_DIRS})
        file(GLOB_RECURSE FILE_LIST "${SRC_DIR}/*")  # Get all files from the source directory

        foreach(FILE ${FILE_LIST})
            # Compute the relative path with respect to the common assets directory (not just SRC_DIR)
            file(RELATIVE_PATH REL_PATH "${GAME_ASSETS_DIR}" "${FILE}")

            # Determine the full destination path, preserving directory structure
            set(DEST_FILE "${DEST_DIR}/${REL_PATH}")

            # Create the destination directory if it doesn't exist
            get_filename_component(DEST_DIR_PATH "${DEST_FILE}" DIRECTORY)
            file(MAKE_DIRECTORY "${DEST_DIR_PATH}")

            # Add the custom command to copy the file if it changed
            add_custom_command(
                OUTPUT ${DEST_FILE}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${DEST_FILE}
                COMMENT "Copying ${REL_PATH} to ${DEST_DIR}"
                DEPENDS ${FILE}
            )

            list(APPEND COPY_TARGETS ${DEST_FILE})
        endforeach()
    endforeach()

    # Create a single target with all copied files as dependencies
    add_custom_target(${TARGET_NAME} ALL DEPENDS ${COPY_TARGETS})
endfunction()
