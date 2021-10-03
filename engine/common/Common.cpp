
#include "common/Precompiled.hpp"
#include "../console/Console.hpp"
#include "Common.hpp"

#include <iostream>

CVar developer( "developer", "0", 0, "Developer mode" );

void Common::Init()
{
	systemTimer.Reset();
}

void Common::Shutdown()
{

}

int Common::DevLevel() const
{
	return developer.GetInt();
}

float Common::Time() const
{	
	return systemTimer.GetElapsed( adm::Timer::Seconds );
}

float Common::TimeMilliseconds() const
{
	return systemTimer.GetElapsed( adm::Timer::Milliseconds );
}

bool Common::IsDedicatedServer() const
{	// No dedicated server code yet
	return false;
}

bool Common::IsHost() const
{	// For now, we only got singleplayer
	return true;
}
