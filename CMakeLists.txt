## Minimum is 3.16 for PCH support
cmake_minimum_required( VERSION 3.16 )

## Project name
project( BurekTechX )

## Some property stuff
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
## C++17's filesystem and inline static initialisers are pretty nice
set( CMAKE_CXX_STANDARD 17 )

## Some cpps include like
## #include "common/Engine.hpp"
## So all projects will typically add this as an include dir
set( BTX_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

## ================================
## Dependencies
## ================================
## SDL2
if ( UNIX )
    find_package( SDL2 REQUIRED )
## On Windows, we got a local copy nearby
elseif ( WIN32 )
    set( SDL2_INCLUDE_DIRS 
        ${BTX_ROOT}/extern/SDL/include )

    set( SDL2_LIBRARIES
        ${BTX_ROOT}/extern/SDL/lib/SDL2.lib 
        ${BTX_ROOT}/extern/SDL/lib/SDL2main.lib )
else()
    message( FATAL_ERROR "This platform is not supported" )
endif()

## SDL2 is being set up here so it can be referenced by both the engine executable and the game library
## Todo: maybe build SDL2 into the engine in the future?

## FTXUI
option( FTXUI_BUILD_TESTS OFF )
option( FTXUI_BUILD_EXAMPLES OFF )
option( FTXUI_BUILD_DOCS OFF )
option( FTXUI_ENABLE_INSTALL OFF )

## The engine really only needs to link against 'component' here,
## so we don't need to link against all 3 individually
add_subdirectory( extern/FTXUI )
set_target_properties( screen PROPERTIES FOLDER "Libs/FTXUI" )
set_target_properties( dom PROPERTIES FOLDER "Libs/FTXUI" )
set_target_properties( component PROPERTIES FOLDER "Libs/FTXUI" )

## Add the engine
add_subdirectory( engine )
