cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( TVPaintAnimationSDK VERSION 1.0.0 LANGUAGES C )

if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
    set( CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE )
endif()

include( GNUInstallDirs )

option( TVPASDK_BUILD_EXAMPLE OFF "Build examples" )

add_library( TVPaintAnimationSDK STATIC src/pi-private.c )
add_library( TVPaintAnimationSDK::TVPaintAnimationSDK ALIAS TVPaintAnimationSDK )

target_include_directories( TVPaintAnimationSDK PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                            )

# Configuration de l’installation
install( DIRECTORY include/TVPaintAnimationSDK/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TVPaintAnimationSDK
         FILES_MATCHING PATTERN "*.h"
         PATTERN "pi-private.h" EXCLUDE
         )

install( TARGETS TVPaintAnimationSDK
         EXPORT TVPaintAnimationSDKTargets
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
         )

install( EXPORT TVPaintAnimationSDKTargets
         FILE TVPaintAnimationSDKTargets.cmake
         NAMESPACE TVPaintAnimationSDK::
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TVPaintAnimationSDK
         )

include( CMakePackageConfigHelpers )
write_basic_package_version_file( "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDKConfigVersion.cmake"
                                  VERSION ${PROJECT_VERSION}
                                  COMPATIBILITY AnyNewerVersion
                                  )

configure_package_config_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TVPaintAnimationSDKConfig.cmake.in"
                               "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDKConfig.cmake"
                               INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TVPaintAnimationSDK
                               )

install( FILES
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDKConfig.cmake"
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDKConfigVersion.cmake"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TVPaintAnimationSDK
         )

# Support pour pkg-config
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TVPaintAnimationSDK.pc.in"
                "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK.pc"
                @ONLY
                )

install( FILES
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK.pc"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
         )

# Configuration de la désinstallation
set( CMAKE_INSTALL_MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt" )

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY
                )

add_custom_target( uninstall
                   COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                   COMMENT "Uninstalling TVPaintAnimationSDK"
                   )

if( TVPASDK_BUILD_EXAMPLE )
    add_subdirectory( examples/PI_Flip )
    add_subdirectory( examples/PI_WaveForm )
endif()
