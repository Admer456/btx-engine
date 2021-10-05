
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

	console->Print( "Input::Init" );

	keys.clear();
	axes.clear();
}

void Input::Shutdown()
{
	console->Print( "Input::Shutdown" );

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
			console->Warning( adm::format( "Input::RegisterKey: Can't register existing key '%s'", key->GetName().data() ) );
			return;
		}
	}

	console->Print( adm::format( "Input::RegisterKey: Registered '%s'", key->GetName().data() ) );
	keys.push_back( key );
}

void Input::RegisterAxis( InputAxis* axis )
{
	for ( auto& localAxis : axes )
	{
		if ( localAxis->GetName() == axis->GetName() )
		{
			console->Warning( adm::format( "Input::RegisterAxis: Can't register existing axis '%s'", axis->GetName().data() ) );
			return;
		}
	}

	axes.push_back( axis );
}
