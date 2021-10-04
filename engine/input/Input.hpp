
#pragma once

class Input : public IInput
{
public:
    void Init() override;
    void Shutdown() override;

    void Update() override;

    void RegisterKey( InputKey* key ) override;
    void RegisterAxis( InputAxis* axis ) override;

private:
    std::vector<InputKey*> keys;
    std::vector<InputAxis*> axes;
};
