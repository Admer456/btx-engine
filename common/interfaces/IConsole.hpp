
#pragma once

class CVarBase;

class IConsole
{
public:
    virtual void    Init() = 0;
    virtual void    Shutdown() = 0;

    // Use adm::format() if you wanna format it
    virtual void    Print( const char* string ) = 0;
    virtual void    DPrint( const char* string, int developerLevel ) = 0;
    virtual void    Warning( const char* string ) = 0;
    virtual void    Error( const char* string ) = 0;

    virtual void    Register( CVarBase* cvar ) = 0;
    virtual bool    Execute( StringRef command, StringRef args ) = 0;

    virtual CVarBase* Find( StringRef name ) = 0;
};
