#pragma once

class IEngine;

class Engine final : public IEngine
{
public:
    void Init( int argc, char** argv ) override;
    void Shutdown() override;

    bool RunFrame() override;

    static Engine& GetInstance()
    {
        static Engine engine;
        return engine;
    }
};
