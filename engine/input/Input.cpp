
#include "common/Precompiled.hpp"
#include "Input.hpp"

#include "SDL.h"

#include "AxisHandler.hpp"

// ============================
// Input::Init
// ============================
bool Input::Init()
{
	console->Print( "Input::Init" );

	// Generate input objects for various keys
	// It goes in ranges
	const std::pair<InputKeyCode::Enum, InputKeyCode::Enum> KeyRanges[] =
	{
		std::make_pair( InputKeyCode::A, InputKeyCode::Z ),
		std::make_pair( InputKeyCode::Number1, InputKeyCode::Number9 ),
		std::make_pair( InputKeyCode::Return, InputKeyCode::Backslash ),
		std::make_pair( InputKeyCode::Semicolon, InputKeyCode::CapsLock ),
		std::make_pair( InputKeyCode::F1, InputKeyCode::F12 ),
		std::make_pair( InputKeyCode::PrintScreen, InputKeyCode::NumLock ),
		std::make_pair( InputKeyCode::KeyPadDivide, InputKeyCode::KeyPadPeriod ),
		std::make_pair( InputKeyCode::NonUSBackslash, InputKeyCode::KeyPadEquals ),
		std::make_pair( InputKeyCode::F13, InputKeyCode::F24 ),
		std::make_pair( InputKeyCode::Execute, InputKeyCode::VolumeDown ),
		std::make_pair( InputKeyCode::KeyPadComma, InputKeyCode::Language9 ),
		std::make_pair( InputKeyCode::SystemRequirements, InputKeyCode::ExSel ),
		std::make_pair( InputKeyCode::KeyPad00, InputKeyCode::KeyPadHexadecimal ),
		std::make_pair( InputKeyCode::LeftCtrl, InputKeyCode::RightGUI )
	};

	keys.reserve( 256U );
	for ( const auto& range : KeyRanges )
	{
		for ( int i = range.first; i <= range.second; i++ )
		{
			keys.emplace_back( i );
		}
	}

	// Generate input objects for various axis codes
	for ( const auto& handler : AxisHandlers )
	{
		axes[handler.axisCode] = InputAxis( handler.axisCode );
		if ( handler.generateDeviceIds )
		{
			for ( int i = 1; i < 4; i++ )
			{
				axes[{handler.axisCode, i}] = InputAxis(handler.axisCode, i);
			}
		}
	}

	return true;
}

// ============================
// Input::Shutdown
// ============================
void Input::Shutdown()
{
	console->Print( "Input::Shutdown" );

	keys.clear();
	axes.clear();
}

// ============================
// Input::Update
// ============================
void Input::Update()
{
	// Axis updates and general event handling
	// TODO: give the user callbacks to handle SDL events in the game DLL
	SDL_Event e;
	while ( SDL_PollEvent( &e ) )
	{
		// Handle window closing
		if ( e.type == SDL_QUIT )
		{
			isWindowClosing = true;
			continue;
		}

		// Try finding all handlers that correspond to this event
		for ( const auto& handler : AxisHandlers )
		{
			if ( handler.sdlEventCode != e.type )
			{
				continue;
			}

			// Update the axis from the event data
			InputAxis& axis = axes[handler.axisCode];
			axis.Update( handler.handlerFunction( e, axis.GetDeviceId() ) );
		}
	}

	UpdateMouseCoordinates();

	// Key updates
	auto states = SDL_GetKeyboardState( nullptr );

	for ( auto& key : keys )
	{
		const int code = key.GetScancode();

		key.Update( code != ScancodeUninitialized ? states[code] : false );
	}
}

// ============================
// Input::GetAxis
// ============================
float Input::GetAxis( InputAxisCode::Enum axis, const int& deviceId ) const
{
	auto iterator = axes.find( { axis, deviceId } );
	if ( iterator == axes.end() )
	{
		return 0.0f;
	}

	return iterator->second.GetValue();
}

// ============================
// Input::GetButton
// ============================
InputKeyFlags Input::GetButton( InputAxisCode::Enum button, const int& deviceId ) const
{
	auto iterator = axes.find( { button, deviceId } );
	if ( iterator == axes.end() )
	{
		return 0;
	}

	return iterator->second.GetState();
}

// ============================
// Input::GetKey
// ============================
InputKeyFlags Input::GetKey( const int& key ) const
{
	for ( const auto& keyObject : keys )
	{
		if ( keyObject.GetScancode() == key )
		{
			return keyObject.GetState();
		}
	}

	return 0;
}

// ============================
// Input::SetMouseGrab
// ============================
void Input::SetMouseGrab( bool grab )
{
	isMouseGrabbed = grab;
	SDL_SetRelativeMouseMode( grab ? SDL_TRUE : SDL_FALSE );
}

// ============================
// Input::IsMouseGrabbed
// ============================
bool Input::IsMouseGrabbed() const
{
	return isMouseGrabbed;
}

// ============================
// Input::IsWindowClosing
// ============================
bool Input::IsWindowClosing() const
{
	return isWindowClosing;
}

// ============================
// Input::UpdateMouseCoordinates
// ============================
void Input::UpdateMouseCoordinates()
{
	int mouseX, mouseY, mouseRelativeX, mouseRelativeY;
	const int mouseState = SDL_GetMouseState( &mouseX, &mouseY );
	SDL_GetRelativeMouseState( &mouseRelativeX, &mouseRelativeY );

	// Need to shorten typing here
	using iac = InputAxisCode;

	axes[iac::MouseX].Update( mouseX );
	axes[iac::MouseY].Update( mouseY );
	axes[iac::MouseXRelative].Update( mouseRelativeX );
	axes[iac::MouseYRelative].Update( mouseRelativeY );
}
