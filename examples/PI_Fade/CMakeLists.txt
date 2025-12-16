cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( PI_Fade )

add_library( ${PROJECT_NAME} MODULE fade.cpp $<$<PLATFORM_ID:Windows>:fade.def> )

target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_20 )

create_plugin_bundle( ${PROJECT_NAME} )

target_include_directories( ${PROJECT_NAME} PRIVATE . )
target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK-CPP )
