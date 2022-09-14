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

## Common .hpps
set( BTX_COMMON_SOURCES 
    ${BTX_ROOT}/common/interfaces/ICore.hpp
    ${BTX_ROOT}/common/interfaces/IConsole.hpp
    ${BTX_ROOT}/common/interfaces/IConsoleListener.hpp
    ${BTX_ROOT}/common/interfaces/IEngine.hpp
    ${BTX_ROOT}/common/interfaces/IFileSystem.hpp
    ${BTX_ROOT}/common/interfaces/IInput.hpp
    ${BTX_ROOT}/common/interfaces/IPluginSystem.hpp
    ${BTX_ROOT}/common/interfaces/plugins/IApplication.hpp
    ${BTX_ROOT}/common/interfaces/plugins/IPlugin.hpp
    ${BTX_ROOT}/common/console/CVar.hpp
    ${BTX_ROOT}/common/console/CVar.cpp
    ${BTX_ROOT}/common/core/IWindow.hpp
    ${BTX_ROOT}/common/pluginsystem/PluginLibrary.hpp
    ${BTX_ROOT}/common/pluginsystem/PluginLibrary.cpp
    ${BTX_ROOT}/common/pluginsystem/PluginLibraryMetadata.hpp
    ${BTX_ROOT}/common/pluginsystem/PluginLibraryMetadata.cpp
    ${BTX_ROOT}/common/pluginsystem/PluginList.hpp
    ${BTX_ROOT}/common/EngineConfig.hpp
    ${BTX_ROOT}/common/EngineConfig.cpp
    ${BTX_ROOT}/common/GameMetadata.hpp
    ${BTX_ROOT}/common/GameMetadata.cpp
    ${BTX_ROOT}/common/Precompiled.hpp 
    CACHE INTERNAL "" )

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

## AdmUtils
add_subdirectory( extern/adm-utils )
set_target_properties( AdmUtils PROPERTIES FOLDER "Libs" )

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
