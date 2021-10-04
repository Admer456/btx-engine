
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
	InputKey( StringRef name, int primaryCode, int secondaryCode = ScancodeUninitialized )
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

class InputAxis : public adm::InstanceChain<InputAxis>
{
public:
	InputAxis() = default;
	InputAxis( StringRef name, int axisCode )
		: actionName( name ), axisType( axisCode )
	{
	}

	constexpr float GetValue() const
	{
		return value;
	}

	StringRef GetName() const
	{
		return actionName;
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
	String	actionName;
	float	value{ 0.0f };
};
