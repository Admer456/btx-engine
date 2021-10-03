
#include "common/Precompiled.hpp"
#include "../console/Console.hpp"
#include "Core.hpp"

CVar developer( "developer", "0", 0, "Developer mode" );

void Core::Init()
{
	systemTimer.Reset();
}

void Core::Shutdown()
{

}

int Core::DevLevel() const
{
	return developer.GetInt();
}

float Core::Time() const
{	
	return systemTimer.GetElapsed( adm::Timer::Seconds );
}

float Core::TimeMilliseconds() const
{
	return systemTimer.GetElapsed( adm::Timer::Milliseconds );
}

bool Core::IsDedicatedServer() const
{	// No dedicated server code yet
	return false;
}

bool Core::IsHost() const
{	// For now, we only got singleplayer
	return true;
}
