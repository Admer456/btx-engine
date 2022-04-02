
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

void ClientGame::Update( const float& deltaTime )
{

}
