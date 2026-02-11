cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( PI_WaveForm LANGUAGES C )

add_library( ${PROJECT_NAME} MODULE
             aadraw.c
             block.c
             waveform.c
             $<$<PLATFORM_ID:Windows>:waveform.def> )

create_plugin_bundle( ${PROJECT_NAME} )
copy_resources_to_bundle( ${PROJECT_NAME}
                          ${CMAKE_CURRENT_SOURCE_DIR}/Resources )

target_include_directories( ${PROJECT_NAME} PRIVATE . )
target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK )
