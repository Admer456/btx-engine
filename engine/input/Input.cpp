
#include "common/Precompiled.hpp"
#include "Input.hpp"

#include "SDL.h"

namespace detail
{
	IInput* gInput = nullptr;
}

// I found this to be the most elegant solution for now
// We do not have many axes, so this is fine and manageable
using AxisCodeHandlerFn = float( SDL_Event& e );
struct AxisCodeHandler
{
	int axisCode{ 0 };
	int sdlEventCode{ 0 };
	AxisCodeHandlerFn* handlerFunction{ nullptr };
};

// We will need to do something about joysticks and gamepads though,
// because there can be multiple of them
constexpr AxisCodeHandler axisHandlers[] =
{
	// Mouse X and Y are automatically handled if it's SDL_MOUSEMOTION
	// We're just here for sake of checking the axis codes
	{ AxisCodes::MouseX, SDL_MOUSEMOTION },
	{ AxisCodes::MouseY, SDL_MOUSEMOTION },
	// Mouse wheel handler is more complete
	{
		AxisCodes::MouseWheel, SDL_MOUSEWHEEL, []( SDL_Event& e ) 
		{ 
			return static_cast<float>(e.wheel.y); 
		}
	}
};

constexpr size_t NumAxisHandlers = sizeof( axisHandlers ) / sizeof( AxisCodeHandler );

// ============================
// Input::Init
// ============================
void Input::Init()
{
	detail::gInput = this;

	console->Print( "Input::Init" );

	keys.clear();
	axes.clear();
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
	// Axis updates
	SDL_Event e;
	while ( SDL_PollEvent( &e ) )
	{
		// Obtain mouse motion here
		if ( e.type == SDL_MOUSEMOTION )
		{
			int mouseX, mouseY;
			SDL_GetMouseState( &mouseX, &mouseY );

			// Update mouse axis handles
			UpdateMouseAxisPointers();

			mouseHorizontalAxis->Update( mouseX );
			mouseVerticalAxis->Update( mouseY );
			continue;
		}

		// Try to find a handler and execute it
		for ( const auto& handler : axisHandlers )
		{
			if ( handler.sdlEventCode == e.type )
			{
				// Now that we found a handler, let's find an axis
				// for it and update it with the needed value
				InputAxis* axis = FindAxis( handler.axisCode );
				if ( nullptr != axis && 
					nullptr != handler.handlerFunction )
				{
					float newValue = handler.handlerFunction( e );
					axis->Update( newValue );
				}

				break;
			}
		}
	}

	// Key updates
	auto states = SDL_GetKeyboardState( nullptr );
	for ( auto& key : keys )
	{
		bool primaryKeyMatch = states[key->GetPrimaryScancode()];
		bool secondaryKeyMatch = key->GetSecondaryScancode() != ScancodeUninitialized;

		if ( secondaryKeyMatch )
		{
			secondaryKeyMatch = states[key->GetSecondaryScancode()];
		}

		key->Update( primaryKeyMatch || secondaryKeyMatch );
	}
}

// ============================
// Input::RegisterKey
// ============================
void Input::RegisterKey( InputKey* key )
{
	for ( auto& localKey : keys )
	{
		if ( localKey->GetName() == key->GetName() )
		{
			console->Warning( adm::format( "Input::RegisterKey: Can't register existing key '%s'", key->GetName().data() ) );
			return;
		}
	}

	console->Print( adm::format( "Input::RegisterKey: Registered '%s'", key->GetName().data() ) );
	keys.push_back( key );
}

// ============================
// Input::RegisterAxis
// ============================
void Input::RegisterAxis( InputAxis* axis )
{
	int code = axis->GetCode();
	if ( code < 0 || code > NumAxisHandlers )
	{
		console->Warning( adm::format( "Input::RegisterAxis: Can't register axis with OOB axis code %i\n", code ) );
		return;
	}

	for ( auto& localAxis : axes )
	{
		if ( localAxis.second->GetCode() == code )
		{
			console->Warning( adm::format( "Input::RegisterAxis: Can't register existing axis ", code ) );
			return;
		}
	}

	axes[code] = axis;
}

// ============================
// Input::UpdateMouse
// ============================
void Input::UpdateMouseAxisPointers()
{
	if ( nullptr == mouseHorizontalAxis )
	{
		mouseHorizontalAxis = FindAxis( AxisCodes::MouseX );
	}

	if ( nullptr == mouseVerticalAxis )
	{
		mouseVerticalAxis = FindAxis( AxisCodes::MouseY );
	}
}

InputAxis* Input::FindAxis( const int& axisCode )
{
	auto find = axes.find( axisCode );
	if ( find != axes.end() )
	{
		return find->second;
	}

	return nullptr;
}
