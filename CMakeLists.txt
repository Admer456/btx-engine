## Minimum is 3.16 for PCH support
cmake_minimum_required( VERSION 3.16 )

## Project name
project( BurekTechX )

## Some property stuff
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
## C++17's filesystem and inline static initialisers are pretty nice
set( CMAKE_CXX_STANDARD 17 )

## UNICODE, UNICODE, UNICODE
if ( WIN32 )
    add_compile_definitions( UNICODE )
    add_compile_definitions( _UNICODE )
endif()

## Options
option( BTX_BUILD_GAME
        "Build game DLLs, disable if you're just writing a sourceport" ON )

## Some cpps include like
## #include "common/Engine.hpp"
## So all projects will typically add this as an include dir
set( BTX_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

## Common .hpps
set( BTX_COMMON_SOURCES 
    ${BTX_ROOT}/common/interfaces/ICore.hpp
    ${BTX_ROOT}/common/interfaces/IConsole.hpp
    ${BTX_ROOT}/common/interfaces/IEngine.hpp
    ${BTX_ROOT}/common/interfaces/IFileSystem.hpp
    ${BTX_ROOT}/common/interfaces/IGame.hpp
    ${BTX_ROOT}/common/interfaces/IInput.hpp
    ${BTX_ROOT}/common/console/CVar.hpp
    ${BTX_ROOT}/common/console/CVar.cpp
    ${BTX_ROOT}/common/GameMetadata.hpp
    ${BTX_ROOT}/common/GameMetadata.cpp
    ${BTX_ROOT}/common/Precompiled.hpp )

## ================================
## Dependencies
## ================================
## SDL2
if ( UNIX )
    find_package( SDL )
## On Windows, we got a local copy nearby
elseif ( WIN32 )
    set( SDLMAIN_LIBRARY 
        ${BTX_ROOT}/extern/SDL/lib/SDL2main.lib )
    
    set( SDL_INCLUDE_DIR 
        ${BTX_ROOT}/extern/SDL/include )

    set( SDL_LIBRARY 
        ${BTX_ROOT}/extern/SDL/lib/SDL2.lib 
        ${BTX_ROOT}/extern/SDL/lib/SDL2main.lib )
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
if ( BTX_BUILD_GAME )
    add_subdirectory( game )
endif()
