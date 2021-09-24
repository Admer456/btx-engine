
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

    // Returns the current time since the app started running
    float       Time() const override;

    // Is this a dedicated server?
    bool        IsDedicatedServer() const override;

private:
    int         developerVerbosity{ 0 };
    adm::Timer  systemTimer;
};
