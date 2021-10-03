
#include "common/Precompiled.hpp"
#include "Console.hpp"

#include <iostream>

extern CVar developer;

IConsole* gConsole = nullptr;

void Console::Init()
{
	gConsole = this;
	CVar::RegisterAll();

	buffer.Init();
}

void Console::Shutdown()
{
	cvarList.clear();
}

void Console::Print( const char* string )
{
	// This is for the dedicated server
	Log( string );

	// Console character buffer for clientside
	// character printing
	buffer.Write( string, core->Time() );
}

void Console::DPrint( const char* string, int developerLevel )
{
	if ( developerLevel >= core->DevLevel() )
	{
		Print( string );
	}
}

void Console::Warning( const char* string )
{
	Print( adm::format( "%sWARNING: %s\n", PrintYellow, string ) );
}

void Console::Error( const char* string )
{
	Print( adm::format( "%sERROR: %s\n", PrintRed, string ) );
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

void Console::Log( const char* string )
{
	char buffer[256];
	size_t position = 0U;
	size_t max = std::min( 256ULL, std::strlen( string ) );

	for ( size_t i = 0U; i < max; i++ )
	{
		// Initiate the skipping
		if ( string[i] == PrintColorIdentifier )
		{
			i += 3; // skip the $xyz sequence
			continue; // does i += 1
		}
		// Store the character
		buffer[position] = string[i];
		position++;
	}

	buffer[position] = '\0';
	std::cout << buffer;
}
