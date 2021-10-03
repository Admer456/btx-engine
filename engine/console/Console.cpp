
#include "common/Precompiled.hpp"
#include "Console.hpp"

#include <iostream>

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
