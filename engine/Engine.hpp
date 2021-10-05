#pragma once

class IEngine;
class IFileSystem;
class FileSystem;

class Engine final : public IEngine
{
public:
    static int  Main( int argc, char** argv );

    void        Init( int argc, char** argv ) override;
    void        Shutdown() override;

    bool        RunFrame() override;

    static bool Command_Mount( StringRef args );
    inline static CVar mount = CVar( "mount", Engine::Command_Mount, "Mounts a game. Usage: mount gameDirectoryName" );

private:
    Console     console;
    Core        core;
    FileSystem  fileSystem;
    Input       input;

    SDL_Window* window{ nullptr };
};
