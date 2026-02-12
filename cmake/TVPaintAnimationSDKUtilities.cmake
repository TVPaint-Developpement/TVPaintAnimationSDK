# ================================================================================================
# create_plugin_bundle
# ================================================================================================
#
# Creates the bundle structure required for a TVPaint Animation plugin.
# TVPaint plugins must be packaged with a .plugin extension and a platform-specific
# directory structure. The bundle name automatically includes the OS suffix:
#   PluginName-MacOS.plugin, PluginName-Windows.plugin, or PluginName-Linux.plugin
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
#     The bundle is then renamed to include the OS suffix via a post-build step.
#     The binary inside the bundle keeps its original name.
#
#     Result: PluginName-MacOS.plugin/Contents/MacOS/PluginName
#
#   Windows:
#     Manually creates the bundle structure via post-build commands:
#     1. Creates the directory: PluginName-Windows.plugin/Contents/Windows/
#     2. Copies the compiled library into this directory
#
#     Result: PluginName-Windows.plugin/Contents/Windows/PluginName.dll
#
#   Linux:
#     Manually creates the bundle structure via post-build commands:
#     1. Creates the directory: PluginName-Linux.plugin/Contents/Linux/
#     2. Copies the compiled library into this directory
#
#     Result: PluginName-Linux.plugin/Contents/Linux/libPluginName.so
#
# Usage example:
#   add_library( MyPlugin SHARED plugin.cpp )
#   create_plugin_bundle( MyPlugin )
#   # Produces: MyPlugin-MacOS.plugin, MyPlugin-Windows.plugin, or MyPlugin-Linux.plugin
#
# ================================================================================================
function( create_plugin_bundle TARGET_NAME )
    if( APPLE )
        set( BUNDLE_OS_SUFFIX "MacOS" )
    elseif( WIN32 )
        set( BUNDLE_OS_SUFFIX "Windows" )
    elseif( LINUX )
        set( BUNDLE_OS_SUFFIX "Linux" )
    else()
        message( FATAL_ERROR "create_plugin_bundle: Unsupported platform. Only MacOS, Windows, and Linux are supported." )
    endif()

    set( BUNDLE_NAME "${TARGET_NAME}-${BUNDLE_OS_SUFFIX}" )

    # Store the bundle name as a custom property so other functions can retrieve it
    set_target_properties( ${TARGET_NAME} PROPERTIES TVPASDK_BUNDLE_NAME "${BUNDLE_NAME}" )

    if( APPLE )
        set_target_properties( ${TARGET_NAME}
                               PROPERTIES
                               BUNDLE YES
                               BUNDLE_EXTENSION plugin )

        add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E rm -rf
                            "${CMAKE_CURRENT_BINARY_DIR}/${BUNDLE_NAME}.plugin"
                            COMMAND ${CMAKE_COMMAND} -E rename
                            "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>"
                            "${CMAKE_CURRENT_BINARY_DIR}/${BUNDLE_NAME}.plugin"
                            COMMENT "Rename bundle to ${BUNDLE_NAME}.plugin" )
    else()
        add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E make_directory
                            "${CMAKE_CURRENT_BINARY_DIR}/${BUNDLE_NAME}.plugin/Contents/${BUNDLE_OS_SUFFIX}"
                            COMMENT "Creation of the bundle structure for ${BUNDLE_NAME}" )

        add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy
                            "$<TARGET_FILE:${TARGET_NAME}>"
                            "${CMAKE_CURRENT_BINARY_DIR}/${BUNDLE_NAME}.plugin/Contents/${BUNDLE_OS_SUFFIX}/"
                            COMMENT "Copy of ${TARGET_NAME} in the bundle structure" )
    endif()
endfunction()

# ================================================================================================
# copy_resources_to_bundle
# ================================================================================================
#
# Copies the contents of a resource directory into the Resources directory of a TVPaint
# plugin bundle. The directory structure is preserved as-is, so the developer must organize
# their source Resources folder to match the expected bundle layout.
#
# Note: create_plugin_bundle() must be called before this function, as it relies on the
#       TVPASDK_BUNDLE_NAME property set by create_plugin_bundle().
#
# Parameters:
#   TARGET_NAME    : Name of the CMake target for the plugin
#   RESOURCES_DIR  : Path to the directory whose contents will be copied into Resources
#
# Platform-specific behavior:
#
#   Copies into: PluginName-{MacOS|Windows|Linux}.plugin/Contents/Resources/
#
# Usage example:
#   copy_resources_to_bundle( MyPlugin
#                              ${CMAKE_CURRENT_SOURCE_DIR}/Resources )
#
# ================================================================================================
function( copy_resources_to_bundle TARGET_NAME RESOURCES_DIR )
    get_target_property( BUNDLE_NAME ${TARGET_NAME} TVPASDK_BUNDLE_NAME )

    if( NOT BUNDLE_NAME )
        message( FATAL_ERROR "copy_resources_to_bundle: create_plugin_bundle() must be called before copy_resources_to_bundle() for target ${TARGET_NAME}" )
    endif()

    set( BUNDLE_RESOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/${BUNDLE_NAME}.plugin/Contents/Resources" )

    add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory
                        "${RESOURCES_DIR}"
                        "${BUNDLE_RESOURCES_DIR}"
                        COMMENT "Copy of resources into the bundle for ${BUNDLE_NAME}" )
endfunction()