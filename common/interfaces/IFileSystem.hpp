
#pragma once

/*
	The folder structure typically goes like this:
	MyGame <- BASE DIRECTORY
		launcher.exe
		engine.dll
		game1 <- FALLBACK DIRECTORY
			gameConfig.json
			plugins
				GameLibrary
					plugin.json
					library.dll
			maps
			models
			sound
		game2 <- GAME DIRECTORY
			gameConfig.json
			plugins
				GameLibrary
					plugin.json
					library.dll
			addons
				MyAddon
					maps
					models
					sound
			maps
			models
			sound

	Every game *must* have a gameConfig.json, describing its name and stuff.
	The typical gameConfig will look like this:

	{
		"gameTitle": "My game",
		"gameDeveloper": "My Gamedev Studio Corp. Inc. LLC GmBH",
		"gamePublisher": "Some Evil Publishing Co."
	}

	Games can "mount" other content as well as have addons.
	It is also possible to mount a relative path like "../OtherGame/game3":

	{
		"mounts": [ "game1", "game2", "../OtherGame/game3" ]
	}
	
	The primary use for mounting would be to have fallback directories,
	in case game 2 depends on some content from game 1.
	The secondary use is for addons. Addon mounting is automatic.
*/

class GameMetadata;

class IFileSystem
{
public:
	enum PathFilterFlags
	{
		Path_Directory = 1,
		Path_File = 2,

		Path_All = Path_Directory | Path_File
	};

	virtual bool Init( Path gameDirectory, Path engineDirectory ) = 0;
	virtual void Shutdown() = 0;

	// @param mountOthers: Mount the games from otherGameDirectory/gameConfig.json?
	// @returns true if it's successfully mounted, false if not
	virtual bool Mount( Path otherGameDirectory, bool mountOthers = false ) = 0;

	// Gets the metadata of the current loaded game
	virtual const GameMetadata& GetCurrentGameMetadata() const = 0;
	// Gets all metadatas of mounted games
	virtual const Vector<GameMetadata>& GetMountedGameMetadatas() const = 0;

	virtual const Path& GetEngineDirectory() const = 0;
	virtual const Path& GetBaseDirectory() const = 0;
	virtual const Path& GetCurrentGameDirectory() const = 0;

	// @param path: The path to be checked
	// @param filterFlags: Is the path a directory, a file, or what?
	// @param noMountedDirectories: Check the path directly
	// @returns true if found in any of the mounted dirs, false if not
	virtual bool Exists( Path path, const uint8_t& filterFlags = Path_All, bool noMountedDirectories = false ) const = 0;

	// @param destination: Relative path to the destination
	// @returns Executable-relative path to the destination in one of the mounted dirs
	virtual adm::Optional<Path> GetPathTo( Path destination, const uint8_t& filterFlags = Path_All, bool noMountedDirectories = false ) const = 0;
};
