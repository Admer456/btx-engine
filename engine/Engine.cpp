#include "common/Precompiled.hpp"

#include "console/Console.hpp"
#include "core/Core.hpp"
#include "filesystem/FileSystem.hpp"
#include "input/Input.hpp"

#include "SDL.h"
#include <iostream>
#include "Engine.hpp"

// ============================
// BTXMain
// extern-declared elsewhere so we don't have
// to include engine headers there
// ============================
int BTXMain( int argc, char** argv )
{
	return Engine::Main( argc, argv );
}

CVar testVar1( "engineTestVar1", "sorgen", CVar_ReadOnly, "This is a test variable" );
CVar testVar2( "engineTestVar2", "200", CVar_ReadOnly, "This is a test variable" );
CVar testVar3( "engineTestVar3", "1", 0, "This is a test variable" );
CVar testVar4( "engineTestVar4", "20.49", 0, "This is a test variable" );

InputKey inputType( "start_typing", SDL_SCANCODE_RETURN );
InputKey inputExit( "quit", SDL_SCANCODE_ESCAPE );

// ============================
// Engine::Main
// The place where it all started...
// ============================
int Engine::Main( int argc, char** argv )
{
	Engine& engine = adm::Singleton<Engine>::GetInstance();

	engine.Init( argc, argv );

	while ( engine.RunFrame() )
	{

	}

	engine.Shutdown();
	return 0;
}

// ============================
// Engine::Init
// ============================
void Engine::Init( int argc, char** argv )
{
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );

	// Timers and stuff
	core.Init();

	// Register static CVars et al
	console.Setup( &core );
	console.Init();
	console.Print( "Initing the engine..." );
	
	// Regsiter keys etc.
	input.Setup( &core, &console );
	input.Init();

	// Ideally this would be done in the client DLL :P
	InputKey::RegisterAll();
	InputAxis::RegisterAll();

	// Initialise the filesystem with the directory of the
	// executable's name and the "base" directory
	Path currentExe = argv[0];
	fileSystem.Setup( &core, &console );
	fileSystem.Init( currentExe.filename().stem() );

	// Raise a window
	window = SDL_CreateWindow( "BurekTech X", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		800, 600, SDL_WINDOW_OPENGL );

	console.Print( adm::format( "Developer level: %i", core.DevLevel() ) );
}

// ============================
// Engine::Shutdown
// Shuts down all subsystems in order of dependency
// ============================
void Engine::Shutdown()
{
	console.Print( "Shutting down..." );

	input.Shutdown();
	fileSystem.Shutdown();
	console.Shutdown();
	core.Shutdown();

	SDL_Quit();
}

// ============================
// Engine::RunFrame
// ============================
bool Engine::RunFrame()
{
	std::this_thread::sleep_for( chrono::milliseconds( 16U ) ); // temporary
	
	// Update the keyboard state etc.
	input.Update();

	// tmporary
	if ( inputType.IsPressed() )
	{
		console.Print( "Enter your command:" );

		String line;
		std::getline( std::cin, line );

		if ( !line.empty() )
		{
			adm::Lexer lex( line );
			String command = lex.Next();
			console.Execute( command, lex.Next() );
		}
	}

	return !inputExit.IsPressed();
}

// ============================
// Engine::Command_Mount
// ============================
bool Engine::Command_Mount( StringRef args )
{
	Engine& self = adm::Singleton<Engine>::GetInstance();

	if ( args.empty() )
	{
		self.console.Warning( "No arguments for mounting" );
		return false;
	}

	self.fileSystem.Mount( args );
	return true;
}
