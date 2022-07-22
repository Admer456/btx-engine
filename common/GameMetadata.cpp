
#include "common/Precompiled.hpp"

GameMetadata::GameMetadata( Path filePath )
{
	json jsonData = adm::ParseJSON( filePath.string() );
	if ( jsonData.empty() )
	{
		return;
	}

	gameName = jsonData.value( "gameName", "unknown" );
	gameDeveloper = jsonData.value( "gameDeveloper", "unknown" );
	gamePublisher = jsonData.value( "gamePublisher", "unknown" );
	gameVersion = jsonData.value( "gameVersion", "unknown" );
	
	for ( const auto& plugin : jsonData["plugins"] )
	{
		pluginLibraries.push_back( plugin );
	}

	for ( const auto& mount : jsonData["mounts"] )
	{
		mountedGames.push_back( mount );
	}

	parsedCorrectly = true;
}
