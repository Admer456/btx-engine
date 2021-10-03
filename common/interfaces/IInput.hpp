
#pragma once

// IInput.hpp
// Input system interface
/*
	Just like CVars, inputs are registered automatically when constructed, like so:
	InputKey forwardKey( "forward", Scancodes::W );
	InputAxisBase mouseY( "mouseY", InputAxes::MouseY );
*/

// Indicators of whether a key was released or not
struct KeyInputState
{
	static constexpr int Released = 0;		// the key has been released for a while
	static constexpr int BecameReleased = 1;// the key was just released this frame
	static constexpr int Held = 2;			// the key has been held for a while
	static constexpr int BecameHeld = 3;	// the key started being held this frame
};

// Identifiers for input axes, this can be anything from gyroscopes to mice
struct InputAxes
{
	static constexpr int MouseX = 0;
	static constexpr int MouseY = 1;
	//static constexpr int ControllerLeftX = 2;
	//static constexpr int ControllerLeftY = 3;
	//...
};

// Defined by: state, name, primary scancode binding, secondary scancode binding
// Multiple keys can have the same bindings (e.g. vehicle forward is W and walk forward is W)
class InputKeyBase;
// Defined by: value, input binding
class InputAxisBase;

class IInput
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	// Polls for input updates
	virtual void Update() = 0;

	// Registers keys and axes for state updating
	virtual void RegisterKey( InputKeyBase* key ) = 0;
	virtual void RegisterAxis( InputAxisBase* axis ) = 0;
};
