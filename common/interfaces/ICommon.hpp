#pragma once

class ICommon
{
public:
    virtual void    Init() = 0;
    virtual void    Shutdown() = 0;

    // Use adm::format() if you wanna format it
    virtual void    Print( const char* string ) = 0;
    virtual void    DPrint( const char* string, int developerLevel ) = 0;
    virtual void    Warning( const char* string ) = 0;
    virtual void    Error( const char* string ) = 0;

    // Developer level
    virtual int     DevLevel() const = 0;

    // Returns the current time since the app started running
    virtual float   Time() const = 0;
    virtual float   TimeMilliseconds() const = 0;

    // Is this a dedicated server?
    // Usually, when this is true, the rendersystem, audiosystem
    // and inputsystem won't be invoked
    virtual bool    IsDedicatedServer() const = 0;
    // Is this instance a server host? (can check for singleplayer with this)
    // @returns false if just a client
    virtual bool    IsHost() const = 0;
};
