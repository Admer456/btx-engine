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

	if ( !engine.Init( argc, argv ) )
	{
		return 1;
	}

	while ( engine.RunFrame() )
	{

	}

	engine.Shutdown( "normal shutdown" );
	return 0;
}

// ============================
// Engine::Init
// ============================
bool Engine::Init( int argc, char** argv )
{
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );

	// Timers and stuff
	core.Init();

	// Register static CVars et al
	console.Setup( &core );
	console.Init( argc, argv );
	console.Print( "Initing the engine..." );
	
	const adm::Dictionary& args = console.GetArguments();

	// Register keys etc.
	input.Setup( &core, &console );
	if ( !input.Init() )
	{
		Shutdown( "input system failure" );
		return false;
	}

	// This must also be done in the game DLL
	InputKey::RegisterAll();
	InputAxis::RegisterAll();

	// Initialise the filesystem with the directory of the
	// game parameter and the "base" directory
	Path currentExe = argv[0];
	// If no -game argument has been passed, use exename_game
	String gameName = args.GetString( "-game" );
	if ( gameName.empty() )
	{
		gameName = currentExe.filename().stem().string() + "_game";
	}

	// Filesystem initialisation
	fileSystem.Setup( &core, &console );
	if ( !fileSystem.Init( gameName ) )
	{
		Shutdown( "filesystem failure" );
		return false;
	}

	// gameName/game.[dll|so]
	if ( !LoadGameLibrary( gameName ) )
	{
		Shutdown( "game library failure" );
		return false;
	}

	// TODO: argument parsing & execution here
	// Things like dedicated server switches, startup CVars etc.

	// TODO: maybe move this to the rendering system
	// TODO: custom window title pls
	console.Print( "Creating a window..." );
	window = SDL_CreateWindow( "BurekTech X", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		800, 600, SDL_WINDOW_OPENGL );

	console.Print( "Window successfully created" );

	console.Print( adm::format( "Developer level: %i", core.DevLevel() ) );

	return true;
}

// ============================
// Engine::Shutdown
// Shuts down all subsystems in order of dependency
// ============================
void Engine::Shutdown( const char* why )
{
	console.Print( adm::format( "Engine: Shutting down, reason: %s", why ) );

	if ( nullptr != serverGame )
	{
		serverGame->Shutdown();
	}

	if ( nullptr != clientGame )
	{
		clientGame->Shutdown();
	}

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
	if ( nullptr != serverGame )
	{
		serverGame->Update( deltaTime );
	}

	// Simulate client frames if not a dedicated server
	if ( nullptr != clientGame )
	{
		clientGame->Update( deltaTime );
	}

	// Normally we'd have more updating stuff here, so syncTimeElapsed would be significantly larger
	// But, if it works, it works
	const int syncTime = (1000.0f / engine_tickRate.GetFloat()) * 1000.0f;
	int syncTimeElapsed = syncTimer.GetElapsed( adm::Timer::Microseconds );
	if ( syncTimeElapsed < syncTime )
	{
		std::this_thread::sleep_for( chrono::microseconds( syncTime - syncTimeElapsed ) );
	}

	deltaTime = syncTimer.GetElapsed( adm::Timer::Milliseconds );
	
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

bool Engine::LoadGameLibrary( StringRef gameName )
{
	// Locating the game DLL
	String gameLibraryPath = String( gameName ) + "/game";
	if constexpr ( adm::Platform == adm::Platforms::Windows )
	{
		gameLibraryPath += ".dll";
	}
	else if constexpr ( adm::Platform == adm::Platforms::Linux )
	{
		gameLibraryPath += ".so";
	}
	else
	{
		console.Error( "Engine: Unsupported platform" );
		return false;
	}

	console.Print( adm::format( "Engine: Loading game library '%s'...", gameLibraryPath.c_str() ) );

	// Obtaining the interface exchange function
	void* gameLibraryHandle = SDL_LoadObject( gameLibraryPath.c_str() );
	if ( nullptr == gameLibraryHandle )
	{
		console.Error( adm::format( "Engine: Can't find game library: ", gameLibraryPath.c_str() ) );
		return false;
	}

	GameInterfaceFunction* gameExchangeFunction = static_cast<GameInterfaceFunction*>(SDL_LoadFunction( gameLibraryHandle, GameInterfaceFunctionName ));
	if ( nullptr == gameExchangeFunction )
	{
		console.Error( adm::format( "Engine: Can't find function '%s' in game library '%s'", GameInterfaceFunctionName, gameLibraryPath.c_str() ) );
		return false;
	}

	// To save myself some typing
	gameLibraryImports& gi = gameImports;
	
	gi.engineVersion = EngineVersion;
	gi.core = &core;
	gi.animation = nullptr;
	gi.collision = nullptr;
	gi.console = &console;
	gi.fileSystem = &fileSystem;
	gi.materialManager = nullptr;
	gi.modelManager = nullptr;
	gi.network = nullptr;
	gi.physics = nullptr;

	gi.audio = nullptr;
	gi.input = &input;
	gi.renderer = nullptr;

	gameLibraryExports* gameExports = gameExchangeFunction( &gameImports );
	if ( nullptr == gameExports )
	{
		console.Error( "Engine: Game library returned nullptr for its interface" );
		return false;
	}

	serverGame = gameExports->server;
	clientGame = gameExports->client;

	if ( nullptr == serverGame )
	{
		console.Error( "Engine: ServerGame is null" );
		return false;
	}
	
	if ( !serverGame->Init() )
	{
		console.Error( "Engine: ServerGame failed to init" );
		return false;
	}

	if ( nullptr == clientGame )
	{
		console.Error( "Engine: ClientGame is null" );
		return false;
	}

	if ( !clientGame->Init() )
	{
		console.Error( "Engine: ClientGame failed to init" );
		return false;
	}

	console.Print( "Engine: Successfully loaded game library" );
	return true;
}
