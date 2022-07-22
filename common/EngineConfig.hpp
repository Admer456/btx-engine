
#pragma once

class EngineConfig final
{
public:
	EngineConfig() = default;
	// @param pathToEngineConfig: the .json file with engine config data
	EngineConfig( Path pathToEngineConfig );
	EngineConfig( const EngineConfig& config) = default;
	EngineConfig( EngineConfig&& config ) = default;

	StringView GetEngineFolder() const { return engineFolder; }
	StringView GetGameFolder() const { return gameFolder; }

	operator bool() const { return parsedCorrectly; }
	EngineConfig& operator=( EngineConfig&& config ) = default;
	EngineConfig& operator=( const EngineConfig& config ) = default;

private:
	String engineFolder;
	String gameFolder;
	bool parsedCorrectly{ false };
};
