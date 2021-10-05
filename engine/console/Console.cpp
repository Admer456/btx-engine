
#include "common/Precompiled.hpp"
#include "Console.hpp"

#include <iostream>

extern CVar developer;

namespace detail
{
	IConsole* gConsole = nullptr;
}

void Console::Init()
{
	detail::gConsole = this;
	CVar::RegisterAll();

	buffer.Init();
}

void Console::Shutdown()
{
	Print( "Console::Shutdown" );
	cvarList.clear();
}

void Console::Print( const char* string )
{
	char* timeString = GenerateTimeString();

	// This is for the dedicated server
	Log( string, timeString );

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
	Print( adm::format( "%sWARNING: %s", PrintYellow, string ) );
}

void Console::Error( const char* string )
{
	Print( adm::format( "%sERROR: %s", PrintRed, string ) );
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
		Warning( adm::format( "Cannot find console command/variable '%s'", command.data() ) );
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

void Console::LogLine( const char* string, const char* timeString )
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

	std::cout << timeString << " | " << buffer << std::endl;
}

void Console::Log( const char* string, const char* timeString )
{
	size_t start = 0;
	size_t end = 0;

	char buffer[256];
	size_t max = std::min( 256ULL, std::strlen( string ) );

	// TODO: turn this into adm::HasNewline
	bool hasNewline = false;
	for ( size_t i = 0U; i < max; i++ )
	{
		if ( string[i] == '\n' )
		{
			hasNewline = true;
			break;
		}
	}

	// Save us the trouble
	if ( !hasNewline )
	{
		return LogLine( string, timeString );
	}

	// Divide the string by newlines, so for example:
	// abc\ndef becomes:
	// abc
	// def
	for ( size_t i = 0U; i < max; i++ )
	{
		if ( string[i] == '\n' && !start && i < max-1 )
		{
			start = i + 1;
			end = 0;
			continue;
		}

		// Special case on the right: there was at least one newline,
		// but there's no newline at the end
		if ( (string[i] == '\n' && !end) || (i == max-2) )
		{
			end = (i == max-2) ? max : i;

			// TODO: turn this into adm::SubString
			size_t s;
			for ( s = 0; s < (end-start); s++ )
			{
				buffer[s] = string[start + s];
			}
			buffer[s] = '\0';

			start = 0;

			// Strip colour data here
			LogLine( buffer, timeString );
		}
	}
}

char* Console::GenerateTimeString()
{
	// hhh:mm:ss.ssss
	static char buffer[32];
	const float time = core->Time();

	int iTime = time;
	int seconds = time;
	int minutes = seconds / 60;
	int hours = minutes / 60;

	minutes = minutes % 60;
	seconds = seconds % 60;

	float flSeconds = seconds + (time - iTime);

	sprintf( buffer, "%02i:%02i:%06.3f", hours, minutes, flSeconds );
	return buffer;
}
