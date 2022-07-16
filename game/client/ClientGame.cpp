
#include "Precompiled.hpp"
#include "ClientGame.hpp"

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

	const float mouseX = Input->GetAxis( InputAxisCode::MouseX );
	const float mouseY = Input->GetAxis( InputAxisCode::MouseY );

	if ( timer > 0.5f )
	{
		timer = 0.0f;
		Console->Print( adm::format( "x: %3.2f, y: %3.2f", mouseX, mouseY ) );
	}

	timer += Core->DeltaTime();
}
