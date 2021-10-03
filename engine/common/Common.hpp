
#pragma once

class ICommon;

class Common : public ICommon
{
public:
    void        Init() override;
    void        Shutdown() override;

    void        Print( const char* string ) override;
    void        DPrint( const char* string, int developerLevel ) override;
    void        Warning( const char* string ) override;
    void        Error( const char* string ) override;

    int         DevLevel() const override;

    // Returns the current time since the app started running
    float       Time() const override;
    float       TimeMilliseconds() const override;

    // Is this a dedicated server?
    bool        IsDedicatedServer() const override;
    // Is this instance a server host?
    bool        IsHost() const override;

private:
    adm::Timer  systemTimer;
};
