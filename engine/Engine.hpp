
#pragma once

class IEngine;
class IFileSystem;
class FileSystem;

class Engine final : public IEngine
{
public:
    static int  Main( int argc, char** argv );

    bool        Init( int argc, char** argv ) override;
    void        Shutdown( const char* why ) override;

    bool        RunFrame() override;

    static bool Command_Mount( StringRef args );
    inline static CVar mount = CVar( "mount", Engine::Command_Mount, "Mounts a game. Usage: mount gameDirectoryName" );

private:
    bool        LoadGameLibrary( StringRef gameName );

private:
    Console     console;
    Core        core;
    FileSystem  fileSystem;
    Input       input;

    gameLibraryImports gameImports;

    float       deltaTime{ 0.0f };
    IGame*      serverGame{ nullptr };
    IGame*      clientGame{ nullptr };

    SDL_Window* window{ nullptr };
};
