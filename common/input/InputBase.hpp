
#pragma once

// A little ugly, but, only declared in engine/input/Input.cpp
// and in game/client/Input.cpp
namespace detail
{
	extern IInput* gInput;
}

class InputKey : public adm::InstanceChain<InputKey>
{
public:
	InputKey() = default;
	InputKey( StringRef name, int primaryCode = ScancodeUninitialized, int secondaryCode = ScancodeUninitialized )
		: actionName( name ), primaryScancode( primaryCode ), secondaryScancode( secondaryCode )
	{
	}

	// constexpr is another way of saying inline
	constexpr int GetState() const 
	{
		return state;
	}

	constexpr int GetPrimaryScancode() const
	{
		return primaryScancode;
	}

	constexpr int GetSecondaryScancode() const
	{
		return secondaryScancode;
	}

	void Update( const bool& pressed )
	{
		if ( pressed )
		{
			if ( IsPressed() )
			{
				state = KeyInputState::Held;
			}
			else
			{
				state = KeyInputState::BecameHeld;
			}
		}
		else
		{
			if ( IsPressed() )
			{
				state = KeyInputState::BecameReleased;
			}
			else
			{
				state = KeyInputState::Released;
			}
		}
	}

	StringRef GetName() const
	{
		return actionName;
	}

	constexpr bool IsPressed() const
	{
		return state == KeyInputState::Held || state == KeyInputState::BecameHeld;
	}

public:
	static void RegisterAll()
	{
		DoAll( []( InputKey* key ) 
		{
			detail::gInput->RegisterKey( key );
		} );
	}

private:
	int		primaryScancode{ ScancodeUninitialized };
	int		secondaryScancode{ ScancodeUninitialized };
	String	actionName;
	int		state{ KeyInputState::Released };
};

struct AxisCodes
{
	// Absolute mouse X in the window, 0 to windowWidth, left to right
	static constexpr int MouseX = 0;
	// Absolute mouse Y in the window, 0 to windowHeight, top to bottom
	static constexpr int MouseY = 1;
	static constexpr int MouseWheel = 2;
};

class InputAxis : public adm::InstanceChain<InputAxis>
{
public:
	InputAxis() = default;
	InputAxis( int axisCode )
		: axisType( axisCode )
	{
	}

	void Update( float newValue )
	{
		value = newValue;
	}

	constexpr int GetCode() const
	{
		return axisType;
	}

	constexpr float GetValue() const
	{
		return value;
	}

public:
	static void RegisterAll()
	{
		DoAll( []( InputAxis* axis )
		{
			detail::gInput->RegisterAxis( axis );
		} );
	}

private:
	int		axisType{ -1 };
	float	value{ 0.0f };
};
