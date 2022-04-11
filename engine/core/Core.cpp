
#include "common/Precompiled.hpp"
#include "../console/Console.hpp"
#include "Core.hpp"

CVar developer( "developer", "0", 0, "Developer mode" );

// ============================
// Core::Init
// ============================
bool Core::Init()
{
	systemTimer.Reset();

	return true;
}

// ============================
// Core::Shutdown
// There's nothing to do here at the moment,
// so shutting down the core would be the safest thing in this codebase
// ============================
void Core::Shutdown()
{

}

// ============================
// Core::DevLevel
// ============================
int Core::DevLevel() const
{
	return developer.GetInt();
}

// ============================
// Core::Time
// ============================
float Core::Time() const
{
	return systemTimer.GetElapsed( adm::Timer::Seconds );
}

// ============================
// Core::TimeMilliseconds
// ============================
float Core::TimeMilliseconds() const
{
	return systemTimer.GetElapsed( adm::Timer::Milliseconds );
}

// ============================
// Core::DeltaTime
// ============================
float Core::DeltaTime() const
{
	return deltaTime;
}

// ============================
// Core::SetDeltaTime
// ============================
void Core::SetDeltaTime( const float& newDeltaTime )
{
	deltaTime = newDeltaTime;
}

// ============================
// Core::IsDedicatedServer
// ============================
bool Core::IsDedicatedServer() const
{	// No dedicated server code yet
	return false;
}

// ============================
// Core::IsHost
// ============================
bool Core::IsHost() const
{	// For now, we only got singleplayer
	return true;
}
