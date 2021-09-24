#include "common/Precompiled.hpp"

#include "common/Common.hpp"
#include "filesystem/FileSystem.hpp"
#include "Engine.hpp"

void Engine::Init( int argc, char** argv )
{
	common.Init();

	fileSystem.Init( "base" );

}

void Engine::Shutdown()
{
	common.Shutdown();
	fileSystem.Shutdown();
}

bool Engine::RunFrame()
{
	return false;
}
