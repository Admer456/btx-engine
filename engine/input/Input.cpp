
#include "common/Precompiled.hpp"
#include "Input.hpp"

#include "SDL.h"

namespace detail
{
	IInput* gInput = nullptr;
}

void Input::Init()
{
	detail::gInput = this;

	keys.clear();
	axes.clear();
}

void Input::Shutdown()
{
	keys.clear();
	axes.clear();
}

void Input::Update()
{
	SDL_Event e;
	while ( SDL_PollEvent( &e ) )
	{
		if ( e.type == SDL_QUIT )
		{

		}
	}

	auto states = SDL_GetKeyboardState( nullptr );
	for ( auto& key : keys )
	{
		bool primaryKeyMatch = states[key->GetPrimaryScancode()] != 0;
		bool secondaryKeyMatch = key->GetSecondaryScancode() != ScancodeUninitialized;
		
		if ( secondaryKeyMatch )
		{
			secondaryKeyMatch = states[key->GetSecondaryScancode()] != 0;
		}

		if ( primaryKeyMatch || secondaryKeyMatch )
		{
			key->Update( true );
		}
	}
}

void Input::RegisterKey( InputKey* key )
{
	for ( auto& localKey : keys )
	{
		if ( localKey->GetName() == key->GetName() )
		{
			//console->Warning
			return;
		}
	}

	keys.push_back( key );
}

void Input::RegisterAxis( InputAxis* axis )
{
	for ( auto& localAxis : axes )
	{
		if ( localAxis->GetName() == axis->GetName() )
		{
			//console->Warning
			return;
		}
	}

	axes.push_back( axis );
}
