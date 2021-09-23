#include "common/interfaces/IEngine.hpp"
#include "Engine.hpp"

#include <SDL.h>

int main( int argc, char** argv )
{
    IEngine& engine = Engine::GetInstance();

    engine.Init( argc, argv );

    while ( engine.RunFrame() )
    {

    }

    engine.Shutdown();
    return 0;
}
