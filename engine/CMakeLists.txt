
## engine/* and commmon/*
set( BTX_ENGINE_SOURCES 
    ${BTX_COMMON_SOURCES}
    common/Common.hpp
    common/Common.cpp
    console/Console.hpp
    console/Console.cpp
    filesystem/FileSystem.hpp
    filesystem/FileSystem.cpp
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
        ${BTX_ROOT}/extern/adm-utils/libs/AdmUtils.lib
        ${SDL_LIBRARY} )

## Precompiled headers
target_precompile_headers( BurekTechX PRIVATE 
        ${BTX_ROOT}/common/Precompiled.hpp )

## Output here
install( TARGETS BurekTechX
        RUNTIME DESTINATION ${BTX_ROOT}/bin/
        LIBRARY DESTINATION ${BTX_ROOT}/bin/ )

if ( WIN32 )
    ## Copy SDL2.dll
    install( FILES ${BTX_ROOT}/extern/SDL/lib/SDL2.dll 
            DESTINATION ${BTX_ROOT}/bin/ )

    ## UNICODE, UNICODE, UNICODE
    add_compile_definitions( UNICODE )
    add_compile_definitions( _UNICODE )

    ## On Windows, place PDBs as well
    install( FILES $<TARGET_PDB_FILE:BurekTechX> DESTINATION ${BTX_ROOT}/bin/ OPTIONAL )
endif()
