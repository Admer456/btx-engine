#include "common/Precompiled.hpp"

#include "common/Common.hpp"
#include "filesystem/FileSystem.hpp"
#include "Engine.hpp"

#include <SDL.h>

int main( int argc, char** argv )
{
    IEngine& engine = adm::Singleton<Engine>::GetInstance();

    engine.Init( argc, argv );

    while ( engine.RunFrame() )
    {

    }

    engine.Shutdown();
    return 0;
}
