# ================================================================================================
# create_plugin_bundle
# ================================================================================================
#
# Creates the bundle structure required for a TVPaint Animation plugin.
# TVPaint plugins must be packaged with a .plugin extension and a platform-specific
# directory structure.
#
# Parameters:
#   TARGET_NAME : Name of the CMake target for the plugin to package
#
# Platform-specific behavior:
#
#   macOS:
#     Automatically configures the target as a native macOS bundle with:
#     - BUNDLE YES : Enables bundle creation
#     - BUNDLE_EXTENSION plugin : Uses .plugin extension instead of .app
#
#     Result: PluginName.plugin/ (native macOS bundle)
#
#   Windows/Linux:
#     Manually creates the bundle structure via post-build commands:
#     1. Creates the directory: PluginName.plugin/Contents/{Windows|Linux}/
#     2. Copies the compiled library into this directory
#
#     Result:
#       Windows: PluginName.plugin/Contents/Windows/PluginName.dll
#       Linux:   PluginName.plugin/Contents/Linux/libPluginName.so
#
# Usage example:
#   add_library( MyPlugin SHARED plugin.cpp )
#   create_plugin_bundle( MyPlugin )
#
# ================================================================================================
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

# ================================================================================================
# copy_resources_to_bundle
# ================================================================================================
#
# Copies resource files (e.g., .loc localization files) into the Resources directory
# of a TVPaint plugin bundle.
#
# Each argument can be:
#   - An explicit file path:  ${CMAKE_CURRENT_SOURCE_DIR}/english.loc
#   - A glob pattern:         ${CMAKE_CURRENT_SOURCE_DIR}/*.loc
#   - A directory:            ${CMAKE_CURRENT_SOURCE_DIR}/resources
#     (all files directly inside the directory will be copied)
#
# Note: Glob patterns and directories are resolved at CMake configure time.
#       New files matching a pattern will only be picked up after re-running CMake.
#
# Parameters:
#   TARGET_NAME              : Name of the CMake target for the plugin
#   [resource files/patterns]: List of file paths, glob patterns, or directories
#
# Platform-specific behavior:
#
#   macOS:
#     Copies files into: PluginName.plugin/Contents/Resources/
#     Uses $<TARGET_BUNDLE_CONTENT_DIR> to resolve the correct bundle path.
#
#   Windows/Linux:
#     Copies files into: PluginName.plugin/Contents/Resources/
#     Uses the binary directory to resolve the bundle path.
#
# Usage examples:
#   # Explicit file list
#   copy_resources_to_bundle( MyPlugin
#                              ${CMAKE_CURRENT_SOURCE_DIR}/english.loc
#                              ${CMAKE_CURRENT_SOURCE_DIR}/french.loc )
#
#   # Glob pattern
#   copy_resources_to_bundle( MyPlugin
#                              ${CMAKE_CURRENT_SOURCE_DIR}/*.loc )
#
#   # Directory containing resources
#   copy_resources_to_bundle( MyPlugin
#                              ${CMAKE_CURRENT_SOURCE_DIR}/resources )
#
#   # Mix of all three
#   copy_resources_to_bundle( MyPlugin
#                              ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
#                              ${CMAKE_CURRENT_SOURCE_DIR}/*.loc
#                              ${CMAKE_CURRENT_SOURCE_DIR}/resources )
#
# ================================================================================================
function( copy_resources_to_bundle TARGET_NAME )
    if( APPLE )
        set( BUNDLE_RESOURCES_DIR "$<TARGET_BUNDLE_CONTENT_DIR:${TARGET_NAME}>/Resources" )
    else()
        set( BUNDLE_RESOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.plugin/Contents/Resources" )
    endif()

    add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory
                        "${BUNDLE_RESOURCES_DIR}"
                        COMMENT "Creation of the Resources directory for ${TARGET_NAME}" )

    # Resolve all arguments into a flat list of files
    set( RESOLVED_FILES )

    foreach( RESOURCE_ENTRY IN LISTS ARGN )
        if( IS_DIRECTORY "${RESOURCE_ENTRY}" )
            # Directory: glob all files directly inside it
            file( GLOB _DIR_FILES "${RESOURCE_ENTRY}/*" )
            foreach( _FILE IN LISTS _DIR_FILES )
                if( NOT IS_DIRECTORY "${_FILE}" )
                    list( APPEND RESOLVED_FILES "${_FILE}" )
                endif()
            endforeach()
        elseif( RESOURCE_ENTRY MATCHES "[*?\\[]" )
            # Glob pattern: expand it
            file( GLOB _GLOB_FILES "${RESOURCE_ENTRY}" )
            foreach( _FILE IN LISTS _GLOB_FILES )
                if( NOT IS_DIRECTORY "${_FILE}" )
                    list( APPEND RESOLVED_FILES "${_FILE}" )
                endif()
            endforeach()
        else()
            # Explicit file path
            list( APPEND RESOLVED_FILES "${RESOURCE_ENTRY}" )
        endif()
    endforeach()

    list( REMOVE_DUPLICATES RESOLVED_FILES )

    foreach( RESOURCE_FILE IN LISTS RESOLVED_FILES )
        cmake_path( GET RESOURCE_FILE FILENAME RESOURCE_FILENAME )

        add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy
                            "${RESOURCE_FILE}"
                            "${BUNDLE_RESOURCES_DIR}/"
                            COMMENT "Copy of ${RESOURCE_FILENAME} in the bundle Resources" )
    endforeach()
endfunction()