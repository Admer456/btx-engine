
#pragma once

// A class that parses gamemeta.txt files
class GameMetadata final
{
public:
	// Assumes that filePath is valid
	GameMetadata( Path filePath );
	~GameMetadata();

	StringRef	GetName() const { return gameName; }
	StringRef	GetDeveloper() const { return gameDeveloper; }
	StringRef	GetPublisher() const { return gamePublisher; }
	StringRef	GetVersion() const { return gameVersion; }

	StringRef	GetMountedGame( const size_t& index );
	size_t		GetNumMountedGames() const;

private:
	void		Parse( File& gameMetaFile );

	String		gameName{ "unknown" };
	String		gameDeveloper{ "unknown" };
	String		gamePublisher{ "unknown" };
	String		gameVersion{ "unknown" };
	std::vector<String> mountedGames{};
};
