// SPDX-FileCopyrightText: 2021-2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"
#include "FileSystem.hpp"

namespace fs = std::filesystem;

// ============================
// FileSystem::Init
// ============================
bool FileSystem::Init( Path gameDirectory, Path engineDirectory )
{
	String gameDirectoryStr = gameDirectory.string();
	
	console->Print( adm::format( "FileSystem::Init: Initialising from base directory '%s'", gameDirectoryStr.c_str() ) );
	
	if ( !fs::exists( gameDirectory ) )
	{
		console->Error( adm::format( "FileSystem::Init: Base game directory '%s' doesn't exist", gameDirectoryStr.c_str() ) );
		return false;
	}

	if ( !fs::exists( gameDirectory/"gameConfig.json" ) )
	{
		console->Error( adm::format( "FileSystem::Init: Base game directory '%s' doesn't have a gameConfig.json", gameDirectoryStr.c_str() ) );
		return false;
	}

	enginePath = engineDirectory;
	basePath = fs::current_path();
	currentGamePath = gameDirectory;

	// The base path contains base engine
	if ( !MountInternal( enginePath, false, false, true ) )
	{
		console->Error( adm::format( "FileSystem::Init: Engine directory '%s' doesn't exist", enginePath.c_str() ) );
		return false;
	}

	// Load game, mounted games & addons
	if ( !MountInternal( currentGamePath, true, true, false ) )
	{
		console->Error( adm::format( "FileSystem::Init: Game directory '%s' doesn't exist", currentGamePath.c_str() ) );
		return false;
	}

	return true;
}

// ============================
// FileSystem::Shutdown
// ============================
void FileSystem::Shutdown()
{
	console->Print( "FileSystem::Shutdown" );
	otherPaths.clear();
}

// ============================
// FileSystem::Mount
// ============================
bool FileSystem::Mount( Path otherGameDirectory, bool mountOthers )
{
	return MountInternal( otherGameDirectory, mountOthers, false, false );
}

// ============================
// FileSystem::GetCurrentGameMetadata
// ============================
const GameMetadata& FileSystem::GetCurrentGameMetadata() const
{
	return gameMetadata;
}

// ============================
// FileSystem::GetMountedGameMetadatas
// ============================
const Vector<GameMetadata>& FileSystem::GetMountedGameMetadatas() const
{
	return otherMetadatas;
}

// ============================
// FileSystem::GetEngineDirectory
// ============================
const Path& FileSystem::GetEngineDirectory() const
{
	return enginePath;
}

// ============================
// FileSystem::GetBaseDirectory
// ============================
const Path& FileSystem::GetBaseDirectory() const
{
	return basePath;
}

// ============================
// FileSystem::GetCurrentGameDirectory
// ============================
const Path& FileSystem::GetCurrentGameDirectory() const
{
	return currentGamePath;
}

// ============================
// FileSystem::Exists
// ============================
bool FileSystem::Exists( Path path, const uint8_t& filterFlags, bool noMountedDirectories ) const
{
	return GetPathTo( path, filterFlags, noMountedDirectories ).has_value();
}

// ============================
// FileSystem::GetPathTo
// ============================
adm::Optional<Path> FileSystem::GetPathTo( Path destination, const uint8_t& filterFlags, bool noMountedDirectories ) const
{
	// If it's absolute or similar, try finding that first
	if ( ExistsInternal( destination, filterFlags ) )
	{
		return destination;
	}

	// Find it in the current game dir
	if ( ExistsInternal( basePath/currentGamePath/destination, filterFlags ) )
	{
		return basePath/currentGamePath/destination;
	}

	if ( noMountedDirectories )
	{
		return {};
	}

	// Check every dependent game dir
	for ( const auto& otherGamePath : otherPaths )
	{
		if ( ExistsInternal( basePath/otherGamePath/destination, filterFlags ) )
		{
			return basePath/otherGamePath/destination;
		}
	}

	// Find it in the engine
	if ( ExistsInternal( basePath / destination, filterFlags ) )
	{
		return basePath / destination;
	}

	// We didn't find it
	return {};
}

// ============================
// FileSystem::MountInternal
// ============================
bool FileSystem::MountInternal( Path otherGameDirectory, bool mountOthers, bool mountingMainGame, bool mountingEngine )
{
	String otherGameDirectoryStr =
		otherGameDirectory.is_absolute() ? // Paths that are already relative can be passed directly
		otherGameDirectory.lexically_relative( basePath ).string() :
		otherGameDirectory.string();

	console->Print( adm::format( "FileSystem::Mount: Mounting '%s'...", otherGameDirectoryStr.c_str() ) );

	if ( !fs::exists( otherGameDirectory ) )
	{
		console->Warning( adm::format( "FileSystem::Mount: Game directory '%s' doesn't exist", otherGameDirectoryStr.c_str() ) );
		return false;
	}

	// Make sure it's unique before proceeding
	for ( const auto& path : otherPaths )
	{
		if ( otherGameDirectory == path )
		{
			return true;
		}
	}

	// The engine does not require a game config
	if ( !mountingEngine )
	{
		GameMetadata gameMeta( otherGameDirectory/"gameConfig.json" );
		if ( !gameMeta )
		{
			console->Warning( adm::format( "FileSystem::Mount: Game directory '%s' doesn't have a gameConfig.json", otherGameDirectoryStr.c_str() ) );
			return false;
		}

		console->Print( adm::format( "FileSystem::Mount: Mounted game '%s'", gameMeta.GetName().data() ) );
		console->Print( adm::format( "                   Developer:   %s", gameMeta.GetDeveloper().data() ) );
		console->Print( adm::format( "                   Publisher:   %s", gameMeta.GetPublisher().data() ) );
		console->Print( adm::format( "                   Version:     %s", gameMeta.GetVersion().data() ) );

		if ( mountingMainGame )
		{
			gameMetadata = gameMeta;
		}
		else
		{
			otherMetadatas.push_back( gameMeta );
			otherPaths.push_back( otherGameDirectory );
		}

		// Mount other games that this one depends on
		if ( mountOthers )
		{
			for ( StringView mountedGame : gameMeta.GetMountedGames() )
			{
				if ( !MountInternal( basePath/mountedGame, false, false, false ) )
				{
					console->Warning( adm::format( "FileSystem::Mount: Can't mount dependency '%s', you may have missing content!", mountedGame.data() ) );
				}
			}
		}
	}

	return true;
}

// ============================
// FileSystem::ExistsInternal
// ============================
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
