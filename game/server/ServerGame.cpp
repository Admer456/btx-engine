
#include "Precompiled.hpp"
#include "ServerGame.hpp"

bool ServerGame::Init()
{
	Console->Print( "ServerGame::Init" );
	return true;
}

void ServerGame::Shutdown()
{
	Console->Print( "ServerGame::Shutdown" );
}

void ServerGame::Update( const float& deltaTime )
{

}
