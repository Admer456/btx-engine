
#include "common/Precompiled.hpp"
#include "Common.hpp"

#include <iostream>

void Common::Init()
{
	systemTimer.Reset();
}

void Common::Print( const char* string )
{
	std::cout << string;
}

void Common::DPrint( const char* string, int developerLevel )
{
	if ( developerLevel >= developerVerbosity )
	{
		Print( string );
	}
}

void Common::Warning( const char* string )
{
	std::cout << "WARNING: " << string;
}

void Common::Error( const char* string )
{
	std::cout << "ERROR: " << string;
}

float Common::Time() const
{	
	return systemTimer.GetElapsed( adm::Timer::Seconds );
}

bool Common::IsDedicatedServer() const
{
	return false;
}
