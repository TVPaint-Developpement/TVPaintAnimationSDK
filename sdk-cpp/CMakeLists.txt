cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( TVPaintAnimationSDK-CPP VERSION 1.0.0 LANGUAGES CXX )

include( GNUInstallDirs )

set( SDK_CPP_SOURCES_FILES
     ${CMAKE_CURRENT_SOURCE_DIR}/include/TVPaintAnimationSDK/*.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/src/TVPaintAnimationSDK/*.cpp )
file( GLOB SDK_CPP_SOURCES LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS ${SDK_CPP_SOURCES_FILES} )

add_library( TVPaintAnimationSDK-CPP STATIC ${SDK_CPP_SOURCES} )
add_library( TVPaintAnimationSDK::TVPaintAnimationSDK-CPP ALIAS TVPaintAnimationSDK-CPP )

set_target_properties( TVPaintAnimationSDK-CPP PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
      POSITION_INDEPENDENT_CODE ON
  )
target_include_directories( TVPaintAnimationSDK-CPP PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                            )
target_link_libraries( TVPaintAnimationSDK-CPP PUBLIC TVPaintAnimationSDK::TVPaintAnimationSDK )

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/TVPaintAnimationSDK/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TVPaintAnimationSDK
         FILES_MATCHING PATTERN "*.hpp"
         )

install( TARGETS TVPaintAnimationSDK-CPP
         EXPORT TVPaintAnimationSDK-CPPTargets
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
         )

install( EXPORT TVPaintAnimationSDK-CPPTargets
         FILE TVPaintAnimationSDK-CPPTargets.cmake
         NAMESPACE TVPaintAnimationSDK::
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TVPaintAnimationSDK
         )

include( CMakePackageConfigHelpers )
write_basic_package_version_file( "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK-CPPConfigVersion.cmake"
                                  VERSION ${PROJECT_VERSION}
                                  COMPATIBILITY AnyNewerVersion
                                  )

configure_package_config_file( "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/TVPaintAnimationSDK-CPPConfig.cmake.in"
                               "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK-CPPConfig.cmake"
                               INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TVPaintAnimationSDK
                               )

install( FILES
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK-CPPConfig.cmake"
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK-CPPConfigVersion.cmake"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TVPaintAnimationSDK
         )

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/TVPaintAnimationSDK-CPP.pc.in"
                "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK-CPP.pc"
                @ONLY
                )

install( FILES
         "${CMAKE_CURRENT_BINARY_DIR}/TVPaintAnimationSDK-CPP.pc"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
         )
