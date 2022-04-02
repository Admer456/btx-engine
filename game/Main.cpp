
#include "Precompiled.hpp"
#include "client/ClientGame.hpp"
#include "server/ServerGame.hpp"

#include <iostream>

// The CVar system needs this for auto-registration
namespace detail
{
	IConsole* gConsole = nullptr;
	CVarList GameCVarList = CVarList();
}

static gameLibraryExports GameExports;
static ClientGame GClient;
static ServerGame GServer;

// Extern "C" prevents name mangling
extern "C" gameLibraryExports* ExchangeGameInterface( gameLibraryImports* engineLibraries )
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

	Engine = engineLibraries;
	detail::gConsole = Engine->console;

	GameExports.client = &GClient;
	GameExports.server = &GServer;

	return &GameExports;
}
