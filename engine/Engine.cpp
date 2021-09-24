#include "common/Precompiled.hpp"

#include "filesystem/FileSystem.hpp"
#include "Engine.hpp"

void Engine::Init( int argc, char** argv )
{
	common->Init();
	fileSystem->Init( "base" );

}

void Engine::Shutdown()
{

}

bool Engine::RunFrame()
{
	return false;
}
