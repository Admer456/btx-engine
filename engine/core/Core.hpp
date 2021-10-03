
#pragma once

class ICore;

class Core : public ICore
{
public:
    void        Init() override;
    void        Shutdown() override;

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
