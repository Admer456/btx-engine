## Minimum is 3.12
cmake_minimum_required( VERSION 3.12 )

## Project name
project( Retrotek )

## Some property stuff
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( CMAKE_CXX_STANDARD 17 ) ## C++17 babyyyy

## Options
option( RETROTEK_BUILD_GAME
        "Build game DLLs, disable if you're just writing a sourceport" ON )

## Some cpps include like
## #include "common/Engine.hpp"
## So all projects will typically add this as an include dir
set( RETROTEK_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

## Common .hpps
set( RETROTEK_COMMON_SOURCES 
    ${RETROTEK_ROOT}/common/interfaces/ICommon.hpp
    ${RETROTEK_ROOT}/common/interfaces/IEngine.hpp
    ${RETROTEK_ROOT}/common/interfaces/IGame.hpp
    ${RETROTEK_ROOT}/common/Precompiled.hpp )

## ================================
## Dependencies
## ================================
## SDL2
if ( UNIX )
    find_package( SDL )
## On Windows, we got a local copy nearby
elseif ( WIN32 )
    set( SDLMAIN_LIBRARY 
        ${RETROTEK_ROOT}/extern/SDL/lib/SDL2main.lib )
    set( SDL_INCLUDE_DIR 
        ${RETROTEK_ROOT}/extern/SDL/include )
    set( SDL_LIBRARY 
        ${RETROTEK_ROOT}/extern/SDL/lib/SDL2.lib 
        ${RETROTEK_ROOT}/extern/SDL/lib/SDL2main.lib )
else()
    message( FATAL_ERROR "This platform is not supported" )
endif()

## AdmUtils
add_subdirectory( extern/adm-utils )
set_target_properties( AdmUtils PROPERTIES
        FOLDER "Libs" )

## ================================
## Engine and game
## ================================
## Add the engine
add_subdirectory( engine )

## Add the game
if ( RETROTEK_BUILD_GAME )
    add_subdirectory( game )
endif()
