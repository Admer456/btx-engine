
## game/*
set( GAME_SOURCES 
    ${BTX_COMMON_SOURCES}

    client/ClientGame.cpp
    client/ClientGame.hpp

    server/ServerGame.cpp
    server/ServerGame.hpp

    Main.cpp
    Precompiled.hpp )

## Folder organisation
source_group( TREE ${BTX_ROOT} FILES ${GAME_SOURCES} )

## Project
add_library( GameLibrary SHARED
    ${GAME_SOURCES} )

## game.dll
set_target_properties( GameLibrary PROPERTIES
    OUTPUT_NAME "game"
    FOLDER "Game" )

## Includes
target_include_directories( GameLibrary PRIVATE
    ${ADMUTIL_INCLUDE_DIRETORY}
    ${BTX_ROOT}/game
    ${BTX_ROOT} )

## Link against adm-utils and whatever
## else you may need here
target_link_libraries( GameLibrary
    AdmUtils )

## Precompiled headers
target_precompile_headers( GameLibrary PRIVATE 
    ${BTX_ROOT}/game/Precompiled.hpp )

## Output here
install( TARGETS GameLibrary
    RUNTIME DESTINATION ${BTX_ROOT}/bin/game/
    LIBRARY DESTINATION ${BTX_ROOT}/bin/game/ )

if ( WIN32 )
    ## Place PDBs on Windows
    install( FILES $<TARGET_PDB_FILE:GameLibrary> 
        DESTINATION ${BTX_ROOT}/bin/game/ OPTIONAL )
endif()
