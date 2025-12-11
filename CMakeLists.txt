cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( TVPaintAnimationSDK VERSION 1.0.0 )

if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
    set( CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE )
endif()

include( cmake/CreatePluginBundle.cmake )
include( GNUInstallDirs )

# Options
option( TVPASDK_BUILD_C_SDK "Build the C SDK" ON )
option( TVPASDK_BUILD_CPP_SDK "Build the C++ SDK wrapper" ON )
if( TVPASDK_BUILD_CPP_SDK )
    message(STATUS "C++ SDK requires C SDK - enabling TVPASDK_BUILD_C_SDK")
    set( TVPASDK_BUILD_C_SDK ON CACHE BOOL "Build the C SDK" FORCE )
endif()
option( TVPASDK_BUILD_EXAMPLE "Build examples" OFF )

# Build the C SDK (required for C++ SDK)
if( TVPASDK_BUILD_C_SDK )
    add_subdirectory( sdk )
endif()

# Build the C++ SDK wrapper (requires C SDK)
if( TVPASDK_BUILD_CPP_SDK )
    add_subdirectory( sdk-cpp )
endif()

# Build examples if requested
if( TVPASDK_BUILD_EXAMPLE )
    if( TVPASDK_BUILD_C_SDK )
        add_subdirectory( examples/PI_Flip )
        add_subdirectory( examples/PI_WaveForm )
    endif()
    if( TVPASDK_BUILD_CPP_SDK )
        add_subdirectory( examples/PI_Demo )
    endif()
endif()

# Uninstallation configuration
set( CMAKE_INSTALL_MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt" )

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY
                )

add_custom_target( uninstall
                   COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                   COMMENT "Uninstalling TVPaintAnimationSDK"
                   )
