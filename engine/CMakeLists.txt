
## engine/* and commmon/*
set( BTX_ENGINE_SOURCES
        assetmanager/Model.hpp
        assetmanager/Model.cpp
        assetmanager/ModelManager.hpp
        assetmanager/ModelManager.cpp
        console/Console.hpp
        console/Console.cpp
        console/ConsoleListenerBasic.cpp
        console/ConsoleListenerInteractive.cpp
        console/ConsoleListenerFileOut.cpp
        console/ftxui/Scroller.hpp
        core/Core.hpp
        core/Core.cpp
        core/VideoFormat.hpp
        core/Window.hpp
        core/Window.cpp
        filesystem/FileSystem.hpp
        filesystem/FileSystem.cpp
        input/AxisHandler.hpp
        input/AxisWithDeviceId.hpp
        input/Input.hpp
        input/Input.cpp
        input/InputObjects.hpp
        pluginsystem/PluginSystem.hpp
        pluginsystem/PluginSystem.cpp
        Engine.hpp
        Engine.cpp
        Engine.Commands.cpp
        Engine.Render.cpp )

## Folder organisation
source_group( TREE ${BTX_ROOT} FILES ${BTX_ENGINE_SOURCES} )

## Project
add_library( BurekTechX SHARED
        ${BTX_ENGINE_SOURCES} )

## btx.exe
set_target_properties( BurekTechX PROPERTIES
        OUTPUT_NAME "BtxEngine"
        FOLDER "Engine" )

## Includes
target_include_directories( BurekTechX PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${BTX_ROOT} )

## Link against SDL2 libs and others
target_link_libraries( BurekTechX 
        AdmUtils
        BtxCommon
        ElegyRhi
        ftxui::component
        ${SDL2_LIBRARIES} )

if ( NVRHI_WITH_VULKAN )
        target_link_libraries( BurekTechX Vulkan-Headers )
endif()

if ( NOT DEFINED BTX_BIN_DIRECTORY )
        set( BTX_BIN_DIRECTORY ${BTX_ROOT}/bin )
endif()

## Output here
install( TARGETS BurekTechX
        RUNTIME DESTINATION ${BTX_BIN_DIRECTORY}
        LIBRARY DESTINATION ${BTX_BIN_DIRECTORY} )

if ( WIN32 )
    ## Copy SDL2.dll
    install( FILES ${BTX_ROOT}/extern/SDL/lib/SDL2.dll
            DESTINATION ${BTX_BIN_DIRECTORY} )

    ## On Windows, place PDBs as well
    install( FILES $<TARGET_PDB_FILE:BurekTechX>
        DESTINATION ${BTX_BIN_DIRECTORY} OPTIONAL )
endif()
