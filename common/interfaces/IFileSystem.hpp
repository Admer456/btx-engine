
#pragma once

/*
	The folder structure typically goes like this:
	MyGame <- BASE DIRECTORY
		launcher.exe
		engine.dll
		game1 <- FALLBACK DIRECTORY
			gamemeta.txt
			game.dll
			maps
			models
			sound
		game2 <- GAME DIRECTORY
			gamemeta.txt
			game.dll
			addons
				MyAddon
					maps
					models 
					sound
			maps
			models
			sound

	Every game *must* have a gamemeta.txt, describing its name and stuff.
	The typical gamemeta.txt will look like this:

	gameTitle "My game"
	gameDeveloper "My Gamedev Studio Corp. Inc. LLC GmBH"
	gamePublisher "Some Evil Publishing Co."

	Games can "mount" other content as well as have addons.
	It is also possible to mount a relative path like "../OtherGame/game3":

	mount "game1"
	mount "game2"
	mount "../OtherGame/game3"

	The primary use for mounting would be to have fallback directories,
	in case game B depends on some content from game A.
*/

class IFileSystem
{
public:
	enum PathFilterFlags
	{
		Path_Directory = 1,
		Path_File = 2,

		Path_All = Path_Directory | Path_File
	};

	virtual bool Init( Path gameDirectory ) = 0;
	virtual void Shutdown() = 0;

	// @param mountOthers: Mount the games from otherGameDirectory/gamemeta.txt?
	// @returns true if it's successfully mounted, false if not
	virtual bool Mount( Path otherGameDirectory, bool mountOthers = false ) = 0;

	virtual const Path& GetEngineDirectory() const = 0;
	virtual const Path& GetBaseDirectory() const = 0;
	virtual const Path& GetCurrentGameDirectory() const = 0;

	// @param path: The path to be checked
	// @param filterFlags: Is the path a directory, a file, or what?
	// @returns true if found in any of the mounted dirs, false if not
	virtual bool Exists( Path path, const uint8_t& filterFlags = Path_All ) const = 0;

	// @param destination: Relative path to the destination
	// @returns Executable-relative path to the destination in one of the mounted dirs
	virtual adm::Optional<Path> GetPathTo( Path destination, const uint8_t& filterFlags = Path_All ) const = 0;
};
