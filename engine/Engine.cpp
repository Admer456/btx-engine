#include "common/Precompiled.hpp"

#include "console/Console.hpp"
#include "core/Core.hpp"
#include "filesystem/FileSystem.hpp"
#include "Engine.hpp"

#include <iostream>

int BTXMain( int argc, char** argv )
{
	return Engine::Main( argc, argv );
}

CVar testVar1( "engineTestVar1", "sorgen", CVar_ReadOnly, "This is a test variable" );
CVar testVar2( "engineTestVar2", "200", CVar_ReadOnly, "This is a test variable" );
CVar testVar3( "engineTestVar3", "1", 0, "This is a test variable" );
CVar testVar4( "engineTestVar4", "20.49", 0, "This is a test variable" );
CVar testCommand( "testCommand", 
				  []( StringRef args ) 
				  { 
					  std::cout << "You called testCommand" << std::endl;
					  return true; 
				  }, 
				  "Does something" );

int Engine::Main( int argc, char** argv )
{
	Engine& engine = adm::Singleton<Engine>::GetInstance();

	engine.Init( argc, argv );

	while ( engine.RunFrame() )
	{
		engine.console.Print( "Ran a frame\n" );
	}

	engine.Shutdown();
	return 0;
}

void Engine::Init( int argc, char** argv )
{
	// Timers and stuff
	core.Init();

	// Register static CVars et al
	console.Init();
	console.Setup( &core );

	console.Print( "Initing the engine...\n" );

	// Initialise the filesystem with the "base" folder
	float startSeconds = core.TimeMilliseconds();
	fileSystem.Init( "base" );
	float endSeconds = core.TimeMilliseconds();

	console.Print( adm::format( "Took %3.5f ms to load 'base'\n", endSeconds - startSeconds ) );

	// Filesystem quick test
	{
		if ( fileSystem.Exists( "base_file.txt", FileSystem::Path_File ) )
		{
			console.Print( "base_file.txt exists\n" );
		}
		else
		{
			console.Print( "base_file.txt doesn't exist\n" );
		}

		if ( fileSystem.Exists( "other_game_file.txt", FileSystem::Path_File ) )
		{
			console.Print( "other_game_file.txt exists\n" );
		}
		else
		{
			console.Print( "other_game_file.txt doesn't exist\n" );
		}

		if ( fileSystem.Exists( "other_game2_file.txt", FileSystem::Path_File ) )
		{
			console.Print( "other_game2_file.txt exists\n" );
		}
		else
		{
			console.Print( "other_game2_file.txt doesn't exist\n" );
		}
	}

	// CVar quick test
	{
		console.Print( adm::format( "Developer level: %i\n", core.DevLevel() ) );

		console.Execute( "engineTestVar1", "" );
		console.Execute( "engineTestVar1", "sargen" );
		console.Execute( "engineTestVar2", "" );
		console.Execute( "engineTestVar2", "300" );
		console.Execute( "engineTestVar3", "" );
		console.Execute( "engineTestVar3", "3" );
		console.Execute( "engineTestVar4", "" );
		console.Execute( "engineTestVar4", "20.50" );
		console.Execute( "testCommand", "" );
	}
}

void Engine::Shutdown()
{
	console.Print( "Shutting down...\n" );

	core.Shutdown();
	console.Shutdown();
	fileSystem.Shutdown();
}

bool Engine::RunFrame()
{
	std::this_thread::sleep_for( chrono::milliseconds( 200 ) );
	return core.Time() < 1.0f;
}
