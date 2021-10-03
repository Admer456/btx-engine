
#pragma once

class CVarBase;

class IConsole
{
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void Register( CVarBase* cvar ) = 0;
    virtual bool Execute( StringRef command, StringRef args ) = 0;

    virtual CVarBase* Find( StringRef name ) = 0;
};
