
#pragma once

struct EngineAPI;

class IEngine
{
public:
	virtual bool Init( int argc, char** argv ) = 0;
	virtual void Shutdown( const char* why ) = 0;

	virtual const EngineAPI& GetAPI() const = 0;

	virtual bool RunFrame() = 0;
};

// Rightmost 2 bytes -> tolerable changes that won't break the game
// Leftmost 2 bytes -> breaking changes that will break the game
//
// If you change anything about the game API or the game imports/exports,
// absolutely change the leftmost engine version bytes
constexpr int EngineVersion = 0x0001;

// The launcher looks for this function
using EngineImportFn = IEngine*();
constexpr const char* EngineImportFunctionName = "GetEngineAPI";
