#include "common/Precompiled.hpp"

#include "common/Common.hpp"
#include "filesystem/FileSystem.hpp"
#include "Engine.hpp"

void Engine::Init( int argc, char** argv )
{
	common = new Common();
	fileSystem = new FileSystem();

	common->Init();
	fileSystem->Init( "base" );

}

void Engine::Shutdown()
{
	if ( nullptr != common )
	{
		delete common;
		common = nullptr;
	}

	if ( nullptr != fileSystem )
	{
		delete fileSystem;
		fileSystem = nullptr;
	}
}

bool Engine::RunFrame()
{
	return false;
}
