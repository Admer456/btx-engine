
#pragma once

// Utilities for key presses and axes

class InputKey
{
public:
	InputKey() = default;
	InputKey( int code = ScancodeUninitialized )
		: scancode( code )
	{
	}

	constexpr int GetState() const
	{
		return state;
	}

	constexpr int GetScancode() const
	{
		return scancode;
	}

	static int GetNextState( const bool& oldPressed, const bool& newPressed )
	{
		int state;

		// Key is currently held down
		if ( newPressed )
		{
			state = InputKeyState::Held;
			// In the previous frame, the key was not being held
			// Therefore, this key has become held
			if ( !oldPressed )
			{
				state |= InputKeyState::BecameHeld;
			}
		}
		// Key is currently released
		else
		{
			state = InputKeyState::Released;
			// In the previous frame, the key was being held
			// Therefore, this key has become released
			if ( oldPressed )
			{
				state |= InputKeyState::BecameReleased;
			}
		}

		return state;
	}

	void Update( const bool& pressed )
	{
		const bool wasPressed = IsPressed();

		state = GetNextState( wasPressed, pressed );
	}

	constexpr bool IsPressed() const
	{
		return state & InputKeyState::Held;
	}

private:
	int scancode{ ScancodeUninitialized };
	InputKeyFlags state{ InputKeyState::Released };
};

class InputAxis
{
public:
	InputAxis() = default;
	InputAxis( int axisCode, int device = 0 )
		: axisType( axisCode ), deviceId( device )
	{
	}

	static constexpr float InvalidValue = -999.0f;

	void Update( float newValue )
	{
		if ( newValue == InvalidValue )
		{
			return;
		}

		const bool wasPressed = state & InputKeyState::Held;
		const bool newPressed = newValue == 1.0f;

		value = newValue;
		state = InputKey::GetNextState( wasPressed, newPressed );
	}

	void ClearImpulseState()
	{
		state &= InputKeyState::Held | InputKeyState::Released;
	}

	constexpr int GetCode() const
	{
		return axisType;
	}

	constexpr int GetState() const
	{
		return state;
	}

	constexpr int GetDeviceId() const
	{
		return deviceId;
	}

	constexpr float GetValue() const
	{
		return value;
	}

private:
	int axisType{ -1 };
	float value{ 0.0f };
	InputKeyFlags state{ InputKeyState::Released };
	int deviceId{ 0 };
};
