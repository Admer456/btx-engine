
## engine/* and commmon/*
set( BTX_ENGINE_SOURCES 
    ${BTX_COMMON_SOURCES}
    common/Common.hpp
    common/Common.cpp
    Engine.hpp
    Engine.cpp
    Main.cpp )

## Folder organisation
source_group( TREE ${BTX_ROOT} FILES ${BTX_ENGINE_SOURCES} )

## BTX.exe
add_executable( BurekTechX
        ${BTX_ENGINE_SOURCES} )

## Output directory
## btx.exe
set_target_properties( BurekTechX PROPERTIES
        OUTPUT_NAME "btx"
        FOLDER "Engine" )

## Includes
target_include_directories( BurekTechX PRIVATE
        ${ADMUTIL_INCLUDE_DIRETORY}
        ${SDL_INCLUDE_DIR}
        ${BTX_ROOT} )

## Link against SDL2 libs and others
target_link_libraries( BurekTechX 
        ${BTX_ROOT}/extern/adm-utils/lib/AdmUtils.lib
        ${SDL_LIBRARY} )

## Output here
install( TARGETS BurekTechX
        RUNTIME DESTINATION ${BTX_ROOT}/bin/
        LIBRARY DESTINATION ${BTX_ROOT}/bin/ )

## Copy SDL2.dll
if ( WIN32 )
    install( FILES ${BTX_ROOT}/extern/SDL/lib/SDL2.dll 
            DESTINATION ${BTX_ROOT}/bin/ )
endif()

## On Windows, place PDBs as well
if ( WIN32 )
    install( FILES $<TARGET_PDB_FILE:BurekTechX> DESTINATION ${BTX_ROOT}/bin/ OPTIONAL )
endif()
