// SPDX-FileCopyrightText: 2021-2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"

#include "console/Console.hpp"
#include "core/Core.hpp"
#include "filesystem/FileSystem.hpp"
#include "input/Input.hpp"
#include "pluginsystem/PluginSystem.hpp"

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
	// Timers and stuff
	bool coreSuccess = core.Init();
	
	// Register static CVars et al
	console.Setup( &core );
	console.Init( argc, argv );
	console.Print( "Initing the engine..." );
	
	// If this somehow happens, congrats
	if ( !coreSuccess )
	{
		console.Error( adm::format( "Core error: %s", core.GetErrorMessage() ) );
		Shutdown( "Core system failure" );
		return false;
	}

	const adm::Dictionary& args = console.GetArguments();

	// Let the core know if this instance is meant to be windowed or not
	core.SetHeadless( args.GetBool( "-headless" ) );

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

	if ( !core.IsHeadless() )
	{
		// Try creating a window
		if ( !CreateWindow() )
		{
			Shutdown( "window failure" );
			return false;
		}
	}

	console.Print( adm::format( "Developer level: %i", core.DevLevel() ) );

	isRunning = true;

	// Start applications now that the engine is fully loaded
	pluginSystem.ForEachPluginOfType<IApplication>( []( IApplication* app )
		{
			app->Start();
		} );

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
	syncTimer.Reset();

	// Update the keyboard state etc.
	input.Update();

	// Update games, apps, tools etc.
	pluginSystem.ForEachPluginOfType<IApplication>( []( IApplication* plugin )
		{
			plugin->Update();
		} );

	// Update console listeners
	console.Update();

	// Normally we'd have more updating stuff here, so syncTimeElapsed would be significantly larger
	// But, if it works, it works
	const int syncTime = (1'000'000.0f / engine_tickRate.GetFloat());
	int syncTimeElapsed = syncTimer.GetElapsed( adm::TimeUnits::Microseconds );
	if ( syncTimeElapsed < syncTime )
	{
		std::this_thread::sleep_for( chrono::microseconds( syncTime - syncTimeElapsed ) );
	}

	deltaTime = syncTimer.GetElapsed( adm::TimeUnits::Seconds );
	core.SetDeltaTime( deltaTime );

	return !input.IsWindowClosing() && !shutdownRequested;
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
// Engine::Command_Quit
// 
// Likely to be called on a separate thread
// ============================
bool Engine::Command_Quit( const ConsoleCommandArgs& args )
{
	adm::Singleton<Engine>::GetInstance().shutdownRequested = true;
	return true;
}

// ============================
// Engine::SetupAPIForExchange
// ============================
void Engine::SetupAPIForExchange()
{
	engineAPI.engineVersion = EngineVersion;
	engineAPI.core = &core;
	engineAPI.console = &console;
	engineAPI.fileSystem = &fileSystem;
	engineAPI.materialManager = nullptr;
	engineAPI.modelManager = nullptr;
	engineAPI.pluginSystem = &pluginSystem;

	if ( !core.IsHeadless() )
	{
		engineAPI.audio = nullptr;
		engineAPI.input = &input;
		engineAPI.renderFrontend = nullptr;
	}
}

// ============================
// Engine::InitialisePlugins
// ============================
bool Engine::InitialisePlugins()
{
	// Load plugins
	{
		bool pluginsFailed = false;
		String pluginErrorString = "Engine::Init: These plugins failed to initialise:\n";
		pluginErrorString.reserve( 256U );

		pluginSystem.ForEachPlugin( [&]( IPlugin* plugin )
			{
				if ( plugin->IsInterface<IApplication>() )
				{
					return;
				}

				if ( !plugin->Init( GetAPI() ) )
				{
					pluginErrorString += "  * " + String( plugin->GetPluginName() ) + "\n";
					pluginsFailed = true;
				}
			} );

		if ( pluginsFailed )
		{
			pluginErrorString += "Resolve the plugin errors and try again.";
			console.Error( pluginErrorString.c_str() );
			return false;
		}
	}

	// Now that base plugins have loaded, it's time to initialise any applications/games
	{
		bool applicationPluginsFailed = false;
		String applicationPluginErrorString = "Engine::Init: These applications failed to initialise:\n";
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
			applicationPluginErrorString += "Resolve the application errors and try again.";
			console.Error( applicationPluginErrorString.c_str() );
			return false;
		}
	}

	return true;
}

// ============================
// Engine::CreateWindow
// ============================
bool Engine::CreateWindow()
{
	WindowCreateDesc windowDesc{ fileSystem.GetCurrentGameMetadata().GetName().data(), 1024, 768};
	windowDesc.resizeable = true;

	console.Print( "Creating a window..." );
	
	mainWindow = core.CreateWindow( windowDesc );
	if ( nullptr == mainWindow )
	{
		console.Error( adm::format( "   * Failed, with error message: '%s'", core.GetErrorMessage() ) );
		return false;
	}

	console.Print( "Window successfully created" );
	return true;
}
