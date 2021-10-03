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

private:
    Console     console;
    Core        core;
    FileSystem  fileSystem;
};
