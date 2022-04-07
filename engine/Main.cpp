
#include <iostream>
#include <SDL.h>

// Look at engine/Engine.cpp
extern int BTXMain( int argc, char** argv );

int main( int argc, char** argv )
{
    std::cout << "BTXMain()" << std::endl;
    if ( BTXMain(argc, argv) )
    {
        std::cout << "BTXMain: An error was encountered" << std::endl;
        return 1;
    }

    std::cout << "BTXMain: Everything went fine" << std::endl;
    return 0;
}
