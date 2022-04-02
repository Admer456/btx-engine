
#pragma once

class Input : public IInput
{
public:
    bool Init() override;
    void Shutdown() override;

    void Update() override;

    void RegisterKey( InputKey* key ) override;
    void RegisterAxis( InputAxis* axis ) override;

    bool IsWindowClosing() const override;

    void Setup( ICore* core, IConsole* console )
    {
        this->core = core;
        this->console = console;
    }

private:
    void UpdateMouseAxisPointers();
    InputAxis* FindAxis( const int& axisCode );

private:
    std::vector<InputKey*> keys;
    std::unordered_map<int, InputAxis*> axes;

    ICore* core{ nullptr };
    IConsole* console{ nullptr };

    InputAxis* mouseHorizontalAxis = nullptr;
    InputAxis* mouseVerticalAxis = nullptr;

    bool isWindowClosing = false;
};
