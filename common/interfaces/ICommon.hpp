#pragma once

class ICommon
{
public:
    // Use va() if you wanna format it
    virtual void Printf( const char* string ) = 0;
    virtual void DPrintf( const char* string, int developerLevel ) = 0;
    virtual void Warning( const char* string ) = 0;
    virtual void Error( const char* string ) = 0;

    // Returns the current time since the app started running
    virtual float Time() const = 0;
};
