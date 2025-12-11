cmake_minimum_required( VERSION 3.25 FATAL_ERROR )
project( PI_Demo )

add_library( ${PROJECT_NAME} MODULE
    demo.cpp
    preview.cpp
    tab-button.cpp
    tab-george.cpp
    tab-meta.cpp
    tab-misc.cpp
    tab-popup.cpp
    tab-preview.cpp
    tab-project.cpp
    tab-requester.cpp
    tab-slider.cpp
    tab-text.cpp
    utilities.cpp
    $<$<PLATFORM_ID:Windows>:demo.def> )

target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_20 )

create_plugin_bundle( ${PROJECT_NAME} )

target_include_directories( ${PROJECT_NAME} PRIVATE . )
target_link_libraries( ${PROJECT_NAME} PRIVATE TVPaintAnimationSDK::TVPaintAnimationSDK-CPP )
