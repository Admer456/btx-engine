
#include "common/Precompiled.hpp"
#include "FileSystem.hpp"

namespace fs = std::filesystem;

// ============================
// FileSystem::Init
// ============================
bool FileSystem::Init( Path gameDirectory )
{
	String gameDirectoryStr = gameDirectory.string();
	
	console->Print( adm::format( "FileSystem::Init: Initialising from base directory '%s'", gameDirectoryStr.c_str() ) );
	
	if ( !fs::exists( gameDirectory ) )
	{
		console->Error( adm::format( "FileSystem::Init: Base game directory '%s' doesn't exist", gameDirectoryStr.c_str() ) );
		return false;
	}

	if ( !fs::exists( gameDirectory/"gamemeta.txt" ) )
	{
		console->Error( adm::format( "FileSystem::Init: Base game directory '%s' doesn't have a gamemeta.txt", gameDirectoryStr.c_str() ) );
		return false;
	}

	basePath = fs::current_path();
	currentGamePath = gameDirectory;

	// Mount the base path
	Mount( "base" );
	// Load addons
	Mount( currentGamePath, true );

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
void FileSystem::Mount( Path otherGameDirectory, bool mountOthers )
{
	String otherGameDirectoryStr =
		otherGameDirectory.is_absolute() ? // Paths that are already relative can be passed directly
		otherGameDirectory.lexically_relative( basePath ).string() :
		otherGameDirectory.string();

	console->Print( adm::format( "FileSystem::Mount: Mounting '%s'...", otherGameDirectoryStr.c_str() ) );

	if ( !fs::exists( otherGameDirectory ) )
	{
		console->Warning( adm::format( "FileSystem::Mount: Game directory '%s' doesn't exist", otherGameDirectoryStr.c_str() ) );
		return;
	}

	// Make sure it has a gamemeta.txt file
	if ( !fs::exists( otherGameDirectory/"gamemeta.txt" ) )
	{
		console->Warning( adm::format( "FileSystem::Mount: Game directory '%s' doesn't have a gamemeta.txt", otherGameDirectoryStr.c_str() ) );
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

	// TODO: maybe pass this as a reference or return it even
	GameMetadata gameMeta( otherGameDirectory/"gamemeta.txt" );

	console->Print( adm::format( "FileSystem::Mount: Mounted game '%s'", gameMeta.GetName().data() ) );
	console->Print( adm::format( "                   Developer:   %s", gameMeta.GetDeveloper().data() ) );
	console->Print( adm::format( "                   Publisher:   %s", gameMeta.GetPublisher().data() ) );
	console->Print( adm::format( "                   Version:     %s", gameMeta.GetVersion().data() ) );

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
			Mount( basePath/gameMeta.GetMountedGame( i ) );
		}
	}
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
