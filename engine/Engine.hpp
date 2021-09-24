#pragma once

class IEngine;
class IFileSystem;
class FileSystem;

class Engine final : public IEngine
{
public:
    void Init( int argc, char** argv ) override;
    void Shutdown() override;

    bool RunFrame() override;

private:
    IFileSystem* fileSystem{ nullptr };
    ICommon* common{ nullptr };
};
