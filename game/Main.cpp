
#include "Precompiled.hpp"
#include "client/ClientGame.hpp"
#include "server/ServerGame.hpp"

#include <iostream>

// The CVar system needs this for auto-registration
namespace detail
{
	CVarList GameCVarList = CVarList();
}

static gameLibraryExports GameExports;
static ClientGame GClient;
static ServerGame GServer;

// Extern "C" prevents name mangling
extern "C" ADM_EXPORT gameLibraryExports* ExchangeGameInterface( gameLibraryImports* engineLibraries )
{
	if ( nullptr == engineLibraries )
	{
		std::cout << "Game::ExchangeGameInterface: engineLibraries null" << std::endl;
		return nullptr;
	}

	if ( engineLibraries->engineVersion != EngineVersion )
	{
		std::cout << "Game::ExchangeGameInterface: engineVersion doesn't match" << std::endl;
		return nullptr;
	}

	// Import stuff from the engine
	Engine = engineLibraries;
	
	// Less typing for game programmers in the end
	Core = Engine->core;
	Animation = Engine->animation;
	Collision = Engine->collision;
	Console = Engine->console;
	FileSystem = Engine->fileSystem;
	MaterialManager = Engine->materialManager;
	ModelManager = Engine->modelManager;
	Network = Engine->network;
	Physics = Engine->physics;

	Audio = Engine->audio;
	Input = Engine->input;
	Renderer = Engine->renderer;

	// Export stuff to the engine
	GameExports.client = &GClient;
	GameExports.server = &GServer;

	return &GameExports;
}
