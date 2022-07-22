#include "common/Precompiled.hpp"

#include "console/Console.hpp"
#include "core/Core.hpp"
#include "filesystem/FileSystem.hpp"
#include "input/Input.hpp"
#include "pluginsystem/PluginSystem.hpp"

#include "SDL.h"
#include "Engine.hpp"

CVar engine_tickRate( "engine_tickRate", "144", 0, "Ticks per second, acts as a framerate cap too" );

// ============================
// GetEngineAPI
// ============================
extern "C" ADM_EXPORT IEngine* GetEngineAPI()
{
	return adm::Singleton<Engine>::GetInstancePtr();
}

// ============================
// Engine::Init
// ============================
bool Engine::Init( int argc, char** argv )
{
	int sdlResult = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );
	
	// Timers and stuff
	core.Init();

	// Register static CVars et al
	console.Setup( &core );
	console.Init( argc, argv );
	console.Print( "Initing the engine..." );
	
	// If this somehow happens, congrats
	if ( sdlResult < 0 )
	{
		console.Error( adm::format( "SDL2 error: %s", SDL_GetError() ) );
		Shutdown( "SDL2 failure" );
		return false;
	}

	const adm::Dictionary& args = console.GetArguments();

	// Load the engine config file
	engineConfig = EngineConfig( "engineConfig.json" );
	if ( !engineConfig )
	{
		Shutdown( "engineConfig.json is missing" );
		return false;
	}

	// Register keys etc.
	input.Setup( &core, &console );
	if ( !input.Init() )
	{
		Shutdown( "input system failure" );
		return false;
	}

	// Initialise the filesystem with the directory of the
	// game parameter and the "base" directory
	Path currentExe = argv[0];
	// If no -game argument has been passed, use engineConfig.json::gameFolder
	String gameName = args.GetString( "-game", engineConfig.GetGameFolder().data() );

	// Filesystem initialisation
	fileSystem.Setup( &core, &console );
	if ( !fileSystem.Init( gameName, engineConfig.GetEngineFolder() ) )
	{
		Shutdown( "filesystem failure" );
		return false;
	}

	// Load gameConfig.json, mount the game's addons and load all needed plugins
	pluginSystem.Setup( &core, &console, &fileSystem );
	if ( !pluginSystem.Init( fileSystem.GetCurrentGameMetadata().GetPluginLibraries() ) )
	{
		Shutdown( "plugin system failure" );
		return false;
	}

	// Initialise pointers for API exchange
	SetupAPIForExchange();

	// Initialise application plugins and give them the engine API
	if ( !InitialisePlugins() )
	{
		Shutdown( "plugin failure" );
		return false;
	}

	// TODO: argument parsing & execution here
	// Things like dedicated server switches, startup CVars etc.

	// TODO: maybe move this to the rendering system
	// TODO: custom window title pls
	console.Print( "Creating a window..." );
	window = SDL_CreateWindow( "BurekTech X", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		800, 600, SDL_WINDOW_RESIZABLE );

	console.Print( "Window successfully created" );

	console.Print( adm::format( "Developer level: %i", core.DevLevel() ) );

	isRunning = true;

	return true;
}

// ============================
// Engine::Shutdown
// Shuts down all subsystems in order of dependency
// ============================
void Engine::Shutdown( const char* why )
{
	if ( !isRunning )
	{
		return;
	}

	console.Print( adm::format( "Engine: Shutting down, reason: %s", why ) );

	pluginSystem.Shutdown();
	input.Shutdown();
	fileSystem.Shutdown();
	console.Shutdown();
	core.Shutdown();

	SDL_Quit();

	isRunning = false;
}

// ============================
// Engine::GetAPI
// ============================
const EngineAPI& Engine::GetAPI() const
{
	return engineAPI;
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

	// Update games, apps, tools etc.
	pluginSystem.ForEachPluginOfType<IApplication>( []( IApplication* plugin )
		{
			plugin->Update();
		} );
	
	// Normally we'd have more updating stuff here, so syncTimeElapsed would be significantly larger
	// But, if it works, it works
	const int syncTime = (1000.0f / engine_tickRate.GetFloat()) * 1000.0f;
	int syncTimeElapsed = syncTimer.GetElapsed( adm::Timer::Microseconds );
	if ( syncTimeElapsed < syncTime )
	{
		std::this_thread::sleep_for( chrono::microseconds( syncTime - syncTimeElapsed ) );
	}

	deltaTime = syncTimer.GetElapsed( adm::Timer::Seconds );
	core.SetDeltaTime( deltaTime );

	return !input.IsWindowClosing();
}

// ============================
// Engine::Command_Mount
// 
// Implementation of the "mount"
// console command
// ============================
bool Engine::Command_Mount( const ConsoleCommandArgs& args )
{
	Engine& self = adm::Singleton<Engine>::GetInstance();

	if ( args.empty() )
	{
		self.console.Warning( "No arguments for mounting" );
		return false;
	}

	self.fileSystem.Mount( args[0] );
	return true;
}

// ============================
// Engine::SetupAPIForExchange
// ============================
void Engine::SetupAPIForExchange()
{
	engineAPI.engineVersion = EngineVersion;
	engineAPI.core = &core;
	engineAPI.animation = nullptr;
	engineAPI.collision = nullptr;
	engineAPI.console = &console;
	engineAPI.fileSystem = &fileSystem;
	engineAPI.materialManager = nullptr;
	engineAPI.modelManager = nullptr;
	engineAPI.network = nullptr;
	engineAPI.physics = nullptr;
	engineAPI.pluginSystem = &pluginSystem;

	engineAPI.audio = nullptr;
	engineAPI.input = &input;
	engineAPI.renderer = nullptr;
}

// ============================
// Engine::InitialisePlugins
// ============================
bool Engine::InitialisePlugins()
{
	bool applicationPluginsFailed = false;
	String applicationPluginErrorString = "Engine::Init: These plugins failed to initialise:\n";
	applicationPluginErrorString.reserve( 256U );

	pluginSystem.ForEachPluginOfType<IApplication>( [&]( IApplication* plugin )
		{
			if ( !plugin->Init( GetAPI() ) )
			{
				applicationPluginErrorString += "  * " + String( plugin->GetPluginName() ) + "\n";
				applicationPluginsFailed = true;
			}
		} );

	if ( applicationPluginsFailed )
	{
		applicationPluginErrorString += "Resolve the plugin errors and try again.";
		console.Error( applicationPluginErrorString.c_str() );
		return false;
	}

	return true;
}
