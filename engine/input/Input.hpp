
#pragma once

struct AxisHandler;

#include "InputObjects.hpp"
#include "AxisWithDeviceId.hpp"

class Input : public IInput
{
public:
	bool Init() override;
	void Shutdown() override;

	void Update() override;

	float GetAxis( InputAxisCode::Enum axis, const int& deviceId ) const override;
	InputKeyFlags GetButton( InputAxisCode::Enum button, const int& deviceId ) const override;
	InputKeyFlags GetKey( const int& key ) const override;

	bool IsWindowClosing() const override;

	void Setup( ICore* core, IConsole* console )
	{
		this->core = core;
		this->console = console;
	}

private:
	void UpdateMouseCoordinates();

private:
	std::vector<InputKey> keys;
	std::unordered_map<AxisWithDeviceId, InputAxis> axes;

	ICore* core{ nullptr };
	IConsole* console{ nullptr };

	bool isWindowClosing = false;
};
