#include "common/Precompiled.hpp"

#include "console/Console.hpp"
#include "core/Core.hpp"
#include "filesystem/FileSystem.hpp"
#include "input/Input.hpp"

#include "SDL.h"
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

CVar engine_tickRate( "engine_tickRate", "144", 0, "Ticks per second, acts as a framerate cap too" );

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
	
	// Register keys etc.
	input.Setup( &core, &console );
	input.Init();

	// This must also be done in the game DLL
	InputKey::RegisterAll();
	InputAxis::RegisterAll();

	// Initialise the filesystem with the directory of the
	// executable's name and the "base" directory
	Path currentExe = argv[0];
	fileSystem.Setup( &core, &console );
	fileSystem.Init( currentExe.filename().stem() );

	// TODO: game DLL loading here
	// Find game DLL, find its interface exchange function,
	// exchange APIs and initialise stuff

	// TODO: argument parsing & execution here
	// Things like dedicated server switches, startup CVars etc.

	// TODO: maybe move this to the rendering system
	// TODO: custom window title pls
	console.Print( "Creating a window..." );
	window = SDL_CreateWindow( "BurekTech X", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		800, 600, SDL_WINDOW_OPENGL );

	console.Print( "Window successfully created" );

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
	// Synchronisation timer, works kinda like V-sync but more flexible
	// Sync time is in microseconds
	adm::Timer syncTimer;

	// Update the keyboard state etc.
	input.Update();

	/*
	// Updating the subsystems is delegated to the game and client.
	// This is partly because I wanna give near full control to the game programmer.
	// This is also because the server and client can have separate "worlds",
	// so to speak. The server has its own physics world, the client has its own, as a
	// consequence of this engine's client-server design.
	//
	// For instance, on the server, rigid bodies may be simulated. On the client, these are
	// received from the server and interpreted as kinematic bodies, and the client can potentially
	// add rigid bodies for particle effects and other things that don't matter to the server.
	//
	// However, if one is writing a fully singleplayer game, then it is possible to simply
	// implement the entire game on the client and call it a day. This is one nice thing about
	// BTX's design IMO -Admer

	// Simulate game frames in SP and MP if you're the host and if the server exports itself
	// The game can simulate multiple frames per engine tick, but also the inverse,
	// depends on the game programmer really
	if ( isHost && nullptr != game )
	{
		// TODO: maybe rename this to serverGame and the latter to clientGame
		game->RunFrame();
	}

	// Simulate client frames if not a dedicated server
	if ( isClient && nullptr != client )
	{
		client->RunFrame();
	}
	*/

	// Normally we'd have more updating stuff here, so syncTimeElapsed would be significantly larger
	// But, if it works, it works
	const int syncTime = (1000.0f / engine_tickRate.GetFloat()) * 1000.0f;
	int syncTimeElapsed = syncTimer.GetElapsed( adm::Timer::Microseconds );
	if ( syncTimeElapsed < syncTime )
	{
		std::this_thread::sleep_for( chrono::microseconds( syncTime - syncTimeElapsed ) );
	}

	const float syncTimePost = syncTimer.GetElapsed( adm::Timer::Milliseconds );

	return !input.IsWindowClosing();
}

// ============================
// Engine::Command_Mount
// 
// Implementation of the "mount"
// console command
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
