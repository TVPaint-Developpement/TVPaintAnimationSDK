cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( PI_Flip LANGUAGES C )

add_library( ${PROJECT_NAME} MODULE flip.c $<$<PLATFORM_ID:Windows>:flip.def> )

create_plugin_bundle( ${PROJECT_NAME} )

target_include_directories( ${PROJECT_NAME} PRIVATE . )
target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK )
