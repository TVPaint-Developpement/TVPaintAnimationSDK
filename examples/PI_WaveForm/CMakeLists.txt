cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( PI_WaveForm )

add_library( ${PROJECT_NAME} MODULE
    aadraw.c
    block.c
    waveform.c
    $<$<PLATFORM_ID:Windows>:waveform.def> )

target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_20 )

if( APPLE )
    set_target_properties( ${PROJECT_NAME}
                           PROPERTIES
                           BUNDLE YES
                           BUNDLE_EXTENSION plugin
                           )
else()
    if( WIN32 )
        set( BUNDLE_ARCH_DIR "Windows" )
    else()
        set( BUNDLE_ARCH_DIR "Linux" )
    endif()

    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory
                        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.plugin/Contents/${BUNDLE_ARCH_DIR}"
                        COMMENT "Creation of the bundle structure for ${PROJECT_NAME}"
                        )

    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "$<TARGET_FILE:${PROJECT_NAME}>"
                        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.plugin/Contents/${BUNDLE_ARCH_DIR}/"
                        COMMENT "Copy of ${PROJECT_NAME} in the bundle structure"
                        )
endif()

target_include_directories( ${PROJECT_NAME} PRIVATE . )
target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK )
