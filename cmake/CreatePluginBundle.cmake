function( create_plugin_bundle TARGET_NAME )
    if( APPLE )
        set_target_properties( ${TARGET_NAME}
                               PROPERTIES
                               BUNDLE YES
                               BUNDLE_EXTENSION plugin )
    else()
        if( WIN32 )
            set( BUNDLE_ARCH_DIR "Windows" )
        else()
            set( BUNDLE_ARCH_DIR "Linux" )
        endif()

        add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E make_directory
                            "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.plugin/Contents/${BUNDLE_ARCH_DIR}"
                            COMMENT "Creation of the bundle structure for ${TARGET_NAME}" )

        add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy
                            "$<TARGET_FILE:${TARGET_NAME}>"
                            "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.plugin/Contents/${BUNDLE_ARCH_DIR}/"
                            COMMENT "Copy of ${TARGET_NAME} in the bundle structure" )
    endif()
endfunction()
