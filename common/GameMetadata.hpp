
#pragma once

// A class that parses gamemeta.txt files
class GameMetadata final
{
public:
	// Assumes that filePath is valid
	GameMetadata( Path filePath );
	~GameMetadata();

	StringView	GetName() const { return gameName; }
	StringView	GetDeveloper() const { return gameDeveloper; }
	StringView	GetPublisher() const { return gamePublisher; }
	StringView	GetVersion() const { return gameVersion; }

	StringView	GetMountedGame( const size_t& index );
	size_t		GetNumMountedGames() const;

private:
	void		Parse( File& gameMetaFile );

	String		gameName{ "unknown" };
	String		gameDeveloper{ "unknown" };
	String		gamePublisher{ "unknown" };
	String		gameVersion{ "unknown" };
	std::vector<String> mountedGames{};
};
