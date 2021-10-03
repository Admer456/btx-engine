
#include "common/Precompiled.hpp"
#include "Console.hpp"

#include <iostream>

extern CVar developer;

IConsole* gConsole = nullptr;

void Console::Init()
{
	gConsole = this;
	CVar::RegisterAll();
}

void Console::Shutdown()
{
	cvarList.clear();
}

void Console::Print( const char* string )
{
	std::cout << string;
}

void Console::DPrint( const char* string, int developerLevel )
{
	if ( developerLevel >= developer.GetInt() )
	{
		Print( string );
	}
}

void Console::Warning( const char* string )
{
	std::cout << "WARNING: " << string;
}

void Console::Error( const char* string )
{
	std::cout << "ERROR: " << string;
}

void Console::Register( CVarBase* cvar )
{
	// Can't have duplicates
	if ( Find( cvar->varName ) )
	{
		return;
	}

	cvarList.push_back( cvar );
}

bool Console::Execute( StringRef command, StringRef args )
{
	CVarBase* cvar = Find( command );
	if ( nullptr == cvar )
	{
		std::cout << "Cannot find " << command << std::endl;
		return false;
	}

	return cvar->Execute( args );
}

CVarBase* Console::Find( StringRef name )
{
	for ( auto& cv : cvarList )
	{
		if ( cv->varName == name )
		{
			return cv;
		}
	}

	return nullptr;
}
