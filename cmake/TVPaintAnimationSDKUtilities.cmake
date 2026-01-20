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
