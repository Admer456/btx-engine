
#pragma once

// A class that parses gameConfig.json files
class GameMetadata final
{
public:
	GameMetadata() = default;
	GameMetadata( Path filePath );
	GameMetadata( const GameMetadata& metadata ) = default;
	GameMetadata( GameMetadata&& metadata ) = default;

	StringView	GetName() const { return gameName; }
	StringView	GetDeveloper() const { return gameDeveloper; }
	StringView	GetPublisher() const { return gamePublisher; }
	StringView	GetVersion() const { return gameVersion; }

	const Vector<String>& GetMountedGames() const { return mountedGames; }
	const Vector<String>& GetPluginLibraries() const { return pluginLibraries; }

	operator bool() const { return parsedCorrectly; }
	GameMetadata& operator=( const GameMetadata& metadata ) = default;
	GameMetadata& operator=( GameMetadata&& metadata ) = default;

private:
	String		gameName;
	String		gameDeveloper;
	String		gamePublisher;
	String		gameVersion;
	Vector<String> mountedGames;
	Vector<String> pluginLibraries;
	bool		parsedCorrectly{ false };
};
