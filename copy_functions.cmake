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