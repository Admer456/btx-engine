
## engine/* and commmon/*
set( RETROTEK_ENGINE_SOURCES 
    ${RETROTEK_COMMON_SOURCES}
    Engine.hpp
    Engine.cpp
    Main.cpp )

## Folder organisation
source_group( TREE ${RETROTEK_ROOT} FILES ${RETROTEK_ENGINE_SOURCES} )

## Retrotek.exe
add_executable( Retrotek
        ${RETROTEK_ENGINE_SOURCES} )

## Output directory
set_target_properties( Retrotek PROPERTIES
        FOLDER "Retrotek" )

## Includes
target_include_directories( Retrotek PRIVATE
        ${SDL_INCLUDE_DIR}
        ${ADMUTIL_INCLUDE_DIRETORY}
        ${RETROTEK_ROOT} )

## Link against SDL2 libs and others
target_link_libraries( Retrotek 
        ${RETROTEK_ROOT}/extern/adm-utils/lib/AdmUtils.lib
        ${SDL_LIBRARY} )

## Output here
install( TARGETS Retrotek
        RUNTIME DESTINATION ${RETROTEK_ROOT}/bin/
        LIBRARY DESTINATION ${RETROTEK_ROOT}/bin/ )
## Copy SDL2.dll
if ( WIN32 )
    install( FILES ${RETROTEK_ROOT}/extern/SDL/lib/SDL2.dll 
            DESTINATION ${RETROTEK_ROOT}/bin/ )
endif()

## On Windows, place PDBs as well
if ( WIN32 )
    install( FILES $<TARGET_PDB_FILE:Retrotek> DESTINATION ${RETROTEK_ROOT}/bin/ OPTIONAL )
endif()
