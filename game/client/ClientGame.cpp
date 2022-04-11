
#include "Precompiled.hpp"
#include "ClientGame.hpp"

// Absolute mouse position inside the window
InputAxis iMouseX( AxisCodes::MouseX );
InputAxis iMouseY( AxisCodes::MouseY );

bool ClientGame::Init()
{
	Console->Print( "ClientGame::Init" );
	return true;
}

void ClientGame::Shutdown()
{
	Console->Print( "ClientGame::Shutdown" );
}

void ClientGame::Update()
{
	static float timer = 0.0f;

	if ( timer > 0.5f )
	{
		timer = 0.0f;
		Console->Print( adm::format( "x: %3.2f, y: %3.2f", iMouseX.GetValue(), iMouseY.GetValue() ) );
	}

	timer += Core->DeltaTime();
}
