
#pragma once

class IEngine;
class IFileSystem;
class FileSystem;

class Engine final : public IEngine
{
public:
	bool		Init( int argc, char** argv ) override;
	void		Shutdown( const char* why ) override;

	const gameLibraryImports& GetAPI() const override;

	bool		RunFrame() override;

	static bool Command_Mount( const ConsoleCommandArgs& args );
	inline static CVar mount = CVar( "mount", Engine::Command_Mount, "Mounts a game. Usage: mount gameDirectoryName" );

private:
	// Populates gameImports with pointers to subsystems
	void		SetupAPIForExchange();

private:
	Console		console;
	Core		core;
	FileSystem	fileSystem;
	Input		input;

	gameLibraryImports gameImports;

	float		deltaTime{ 0.0f };
	bool		isRunning{ false };

	SDL_Window* window{ nullptr };
};
