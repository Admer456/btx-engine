// SPDX-FileCopyrightText: 2021-2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

namespace nvrhi::app
{
	class DeviceManager;
}

class Engine final : public IEngine
{
public:
	bool		Init( int argc, char** argv ) override;
	void		Shutdown( const char* why ) override;

	const EngineAPI& GetAPI() const override;

	bool		RunFrame() override;

	static bool Command_Mount( const ConsoleCommandArgs& args );
	inline static CVar mount = CVar( "mount", Engine::Command_Mount, "Mounts a game. Usage: mount gameDirectoryName" );

	static bool Command_Quit( const ConsoleCommandArgs& args );
	inline static CVar quit = CVar( "quit", Engine::Command_Quit, "Quits the game." );

private:
	// Populates engineAPI with pointers to subsystems
	void		SetupAPIForExchange();
	// Once loaded, this will initialise all plugins
	bool		InitialisePlugins();
	// Initialise IApplication plugins
	bool		InitialiseApplications();
	// Creates the main application window
	bool		CreateWindow();

private: // Renderer backend stuff (Engine.Render.cpp)
	// Initialises the render frontend plugin
	// Not called in headless mode
	bool		InitialiseRenderer();
	// Creates a swapchain for mainWindow
	// Not in headless mode
	bool		CreateDeviceAndSwapchain();

private:
	Console		console;
	Core		core;
	FileSystem	fileSystem;
	Input		input;
	PluginSystem pluginSystem;
	IRenderFrontend* renderFrontend{ nullptr };
	nvrhi::app::DeviceManager* renderBackendManager{ nullptr };

	EngineAPI	engineAPI;
	EngineConfig engineConfig;

	// Synchronisation timer, works kinda like V-sync but more flexible
	// Sync time is in microseconds
	adm::TimerPreciseDouble syncTimer;
	float		deltaTime{ 0.0f };
	bool		isRunning{ false };
	bool		shutdownRequested{ false };

	IWindow*	mainWindow{ nullptr };
};
