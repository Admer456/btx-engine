
#include "Precompiled.hpp"
#include "ServerGame.hpp"

CVar g_difficulty( "g_difficulty", "0" );

bool ServerGame::Init()
{
	Console->Print( "ServerGame::Init" );
	Console->Print( adm::format( "g_difficulty is %i", g_difficulty.GetInt() ) );
	return true;
}

void ServerGame::Shutdown()
{
	Console->Print( "ServerGame::Shutdown" );
}

void ServerGame::Update()
{

}
