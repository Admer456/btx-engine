#pragma once

class ICommon
{
public:
    virtual void    Init() = 0;

    // Use adm::format() if you wanna format it
    virtual void    Print( const char* string ) = 0;
    virtual void    DPrint( const char* string, int developerLevel ) = 0;
    virtual void    Warning( const char* string ) = 0;
    virtual void    Error( const char* string ) = 0;

    // Returns the current time since the app started running
    virtual float   Time() const = 0;

    // Is this a dedicated server?
    virtual bool    IsDedicatedServer() const = 0;
};
