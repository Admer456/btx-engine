
## engine/* and commmon/*
set( BTX_ENGINE_SOURCES 
    ${BTX_COMMON_SOURCES}
    console/Console.hpp
    console/Console.cpp
    core/Core.hpp
    core/Core.cpp
    console/ConsoleBuffer.hpp
    console/ConsoleBuffer.cpp
    filesystem/FileSystem.hpp
    filesystem/FileSystem.cpp
    input/AxisHandler.hpp
    input/AxisWithDeviceId.hpp
    input/Input.hpp
    input/Input.cpp
    input/InputObjects.hpp
    Engine.hpp
    Engine.cpp
    Main.cpp )

## Folder organisation
source_group( TREE ${BTX_ROOT} FILES ${BTX_ENGINE_SOURCES} )

## Project
add_executable( BurekTechX
        ${BTX_ENGINE_SOURCES} )

## btx.exe
set_target_properties( BurekTechX PROPERTIES
        OUTPUT_NAME "btx"
        FOLDER "Engine" )

## Includes
target_include_directories( BurekTechX PRIVATE
        ${ADMUTIL_INCLUDE_DIRETORY}
        ${SDL2_INCLUDE_DIRS}
        ${BTX_ROOT} )

## Link against SDL2 libs and others
target_link_libraries( BurekTechX 
        AdmUtils
        ${SDL2_LIBRARIES} )

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

    ## On Windows, place PDBs as well
    install( FILES $<TARGET_PDB_FILE:BurekTechX> 
        DESTINATION ${BTX_ROOT}/bin/ OPTIONAL )
endif()
