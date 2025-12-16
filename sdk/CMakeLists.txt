cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( TVPaintAnimationSDK-C VERSION 1.0.0 LANGUAGES C )

include( GNUInstallDirs )

add_library( TVPaintAnimationSDK STATIC src/TVPaintAnimationSDK/pi-private.c )
add_library( TVPaintAnimationSDK::TVPaintAnimationSDK ALIAS TVPaintAnimationSDK )

set_target_properties( TVPaintAnimationSDK PROPERTIES
      C_STANDARD 11
      C_STANDARD_REQUIRED ON
      C_EXTENSIONS OFF
      POSITION_INDEPENDENT_CODE ON
  )
target_include_directories( TVPaintAnimationSDK PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                            )

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/TVPaintAnimationSDK/
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

configure_package_config_file( "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/TVPaintAnimationSDKConfig.cmake.in"
                               "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDKConfig.cmake"
                               INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TVPaintAnimationSDK
                               )

install( FILES
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDKConfig.cmake"
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDKConfigVersion.cmake"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TVPaintAnimationSDK
         )

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/TVPaintAnimationSDK.pc.in"
                "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK.pc"
                @ONLY
                )

install( FILES
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK.pc"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
         )

