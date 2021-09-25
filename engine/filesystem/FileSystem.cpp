
#include "common/Precompiled.hpp"
#include "FileSystem.hpp"

namespace fs = std::filesystem;

void FileSystem::Init( Path gameDirectory )
{
	if ( !fs::exists( gameDirectory ) )
	{
		//gEngine.Common.Error( "Game directory doesn't exist" );
		return;
	}

	if ( !fs::exists( gameDirectory/"gamemeta.txt" ) )
	{
		//gEngine.Common.Error( "Game directory doesn't have a gamemeta.txt" );
		return;
	}

	basePath = fs::current_path();
	currentGamePath = gameDirectory;

	// Load addons
	Mount( currentGamePath, true );
}

void FileSystem::Shutdown()
{
	otherPaths.clear();
}

void FileSystem::Mount( Path otherGameDirectory, bool mountOthers )
{
	if ( !fs::exists( otherGameDirectory ) )
	{
		//std::string otherGame = otherGameDirectory.string();
		//gEngine.Common.Warning( adm::format( "Game directory '%s' doesn't exist", otherGame.c_str() ) );
		return;
	}

	// Make sure it has a gamemeta.txt file
	if ( !fs::exists( otherGameDirectory/"gamemeta.txt" ) )
	{
		//gEngine.Common.Warning( "Game directory doesn't have a gamemeta.txt" );
		return;
	}

	// Make sure it's unique before proceeding
	for ( const auto& path : otherPaths )
	{
		if ( otherGameDirectory == path )
		{
			return;
		}
	}

	GameMetadata gameMeta( otherGameDirectory/"gamemeta.txt" );
	
	printf( "FileSystem::Mount: Mounted game '%s'\n", gameMeta.GetName().data() );
	printf( "                   Developer: %s\n", gameMeta.GetDeveloper().data() );
	printf( "                   Publisher: %s\n", gameMeta.GetPublisher().data() );
	printf( "                   Version: %s\n\n", gameMeta.GetVersion().data() );

	// Add it
	if ( otherGameDirectory != currentGamePath )
	{
		otherPaths.push_back( otherGameDirectory );
	}

	// Mount other games that this one depends on
	if ( mountOthers )
	{
		for ( size_t i = 0; i < gameMeta.GetNumMountedGames(); i++ )
		{
			Mount( basePath/gameMeta.GetMountedGame(i) );
		}
	}
}

const Path& FileSystem::GetBaseDirectory() const
{
	return basePath;
}

const Path& FileSystem::GetCurrentGameDirectory() const
{
	return currentGamePath;
}

bool FileSystem::Exists( Path path, const uint8_t& filterFlags ) const
{
	// If it's absolute or similar, try finding that first
	if ( ExistsInternal( path, filterFlags ) )
	{
		return true;
	}

	// Find it in the engine
	if ( ExistsInternal( basePath/path, filterFlags ) )
	{
		return true;
	}

	// Find it in the current game dir
	if ( ExistsInternal( basePath/currentGamePath/path, filterFlags ) )
	{
		return true;
	}

	// Check every dependent game dir
	for ( const auto& otherGamePath : otherPaths )
	{
		if ( ExistsInternal( basePath/otherGamePath/path, filterFlags ) )
		{
			return true;
		}
	}

	// We didn't find it...
	return false;
}

bool FileSystem::ExistsInternal( Path path, const uint8_t& filterFlags ) const
{
	bool exists = fs::exists( path );
	uint8_t fileFlags = 0;

	if ( fs::is_directory( path ) )
	{
		fileFlags |= Path_Directory;
	}

	if ( fs::is_regular_file( path ) )
	{
		fileFlags |= Path_File;
	}

	return exists && (filterFlags & fileFlags);
}
