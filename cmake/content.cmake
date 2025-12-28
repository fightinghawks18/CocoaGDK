function(copy_content_directory TARGET CONTENT_DIR)
    get_target_property(TARGET_NAME ${TARGET} NAME)
    add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${CONTENT_DIR}
            $<TARGET_FILE_DIR:${TARGET}>/${CONTENT_DIR}
            COMMENT "Copying ${TARGET_NAME}'s content to output"
    )
endfunction(copy_content_directory TARGET CONTENT_DIR)