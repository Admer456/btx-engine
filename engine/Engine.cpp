#include "common/Precompiled.hpp"

#include "common/Common.hpp"
#include "filesystem/FileSystem.hpp"
#include "Engine.hpp"

void Engine::Init( int argc, char** argv )
{
	common.Init();

	float startSeconds = common.TimeMilliseconds();
	fileSystem.Init( "base" );
	float endSeconds = common.TimeMilliseconds();

	common.Print( adm::format( "Took %3.5f ms to load 'base'\n", endSeconds - startSeconds ) );

	if ( fileSystem.Exists( "base_file.txt", IFileSystem::Path_File ) )
	{
		common.Print( "base_file.txt exists\n" );
	}
	else
	{
		common.Print( "base_file.txt doesn't exist\n" );
	}

	if ( fileSystem.Exists( "other_game_file.txt", IFileSystem::Path_File ) )
	{
		common.Print( "other_game_file.txt exists\n" );
	}
	else
	{
		common.Print( "other_game_file.txt doesn't exist\n" );
	}

	if ( fileSystem.Exists( "other_game2_file.txt", IFileSystem::Path_File ) )
	{
		common.Print( "other_game2_file.txt exists\n" );
	}
	else
	{
		common.Print( "other_game2_file.txt doesn't exist\n" );
	}
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
