#include "common/Precompiled.hpp"

#include "common/Common.hpp"
#include "filesystem/FileSystem.hpp"
#include "Engine.hpp"

#include <SDL.h>
#include <iostream>

int main( int argc, char** argv )
{
    IEngine& engine = adm::Singleton<Engine>::GetInstance();

    for ( int i = 0; i < 3; i++ )
    {
        std::cout << "Waiting to attach debugger (" << 3 - i << " seconds)..." << std::endl;
        std::this_thread::sleep_for( chrono::milliseconds( 1000 ) );
    }

    std::cout << "Initing the engine..." << std::endl;
    engine.Init( argc, argv );

    while ( engine.RunFrame() )
    {
        std::cout << "Ran a frame" << std::endl;
    }

    std::cout << "Shutting down..." << std::endl;
    engine.Shutdown();
    return 0;
}
