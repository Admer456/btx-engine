
#pragma once

class IEngine
{
public:
    virtual void Init( int argc, char** argv ) = 0;
    virtual void Shutdown() = 0;

    virtual bool RunFrame() = 0;
};
