
#include "common/Precompiled.hpp"
#include "Console.hpp"

#include <iostream>

// ============================
// Console::Init
// Initialises engine CVars, game CVars are initialised separately
// ============================
bool Console::Init( int argc, char** argv )
{
	ParseArguments( argc, argv );

	EngineConsole = this;
	CVar::RegisterAll();

	buffer.Init();

	cvarList.reserve( 1024U );

	return true;
}

// ============================
// Console::Shutdown
// ============================
void Console::Shutdown()
{
	Print( "Console::Shutdown" );
	CVar::UnregisterAll();
	cvarList.clear();
	arguments.Clear();
}

// ============================
// Console::Print
// ============================
void Console::Print( const char* string )
{
	char* timeString = GenerateTimeString();

	// This is for the dedicated server
	Log( string, timeString );

	// Console character buffer for clientside
	// character printing
	buffer.Write( string, core->Time() );
}

// ============================
// Console::DPrint
// ============================
void Console::DPrint( const char* string, int developerLevel )
{
	if ( developerLevel >= core->DevLevel() )
	{
		Print( string );
	}
}

// ============================
// Console::Warning
// ============================
void Console::Warning( const char* string )
{
	Print( adm::format( "%sWARNING: %s", PrintYellow, string ) );
}

// ============================
// Console::Error
// ============================
void Console::Error( const char* string )
{
	Print( adm::format( "%sERROR: %s", PrintRed, string ) );
}

// ============================
// Console::Register
// ============================
void Console::Register( CVarBase* cvar )
{
	if ( nullptr == cvar )
	{
		return;
	}

	// Can't have duplicates
	if ( Find( cvar->varName ) )
	{
		return;
	}

	cvarList[cvar->varName] = cvar;
}

// ============================
// Console::Unregister
// ============================
void Console::Unregister( CVarBase* cvar )
{
	// Can't have duplicates
	if ( !Find( cvar->varName ) )
	{
		return;
	}

	cvarList.erase( cvar->varName );
}

// ============================
// Console::Execute
// ============================
bool Console::Execute( StringView command, StringView args )
{
	Lexer lex( args );
	lex.SetDelimiters( Lexer::DelimitersSimple );

	// Simple parsing
	ConsoleCommandArgs commandArgs;
	while ( !lex.IsEndOfFile() )
	{
		String token = lex.Next();
		if ( token.empty() )
		{
			break;
		}

		commandArgs.push_back( lex.Next() );
	}

	CVarBase* cvar = Find( command );
	if ( nullptr == cvar )
	{
		Warning( adm::format( "Cannot find console command/variable '%s'", command.data() ) );
		return false;
	}

	return cvar->Execute( commandArgs, this );
}

// ============================
// Console::Execute
// ============================
bool Console::Execute( StringView command, const Vector<StringView>& args )
{
	CVarBase* cvar = Find( command );
	if ( nullptr == cvar )
	{
		Warning( adm::format( "Cannot find console command/variable '%s'", command.data() ) );
		return false;
	}

	return cvar->Execute( args, this );
}

// ============================
// Console::Find
// ============================
CVarBase* Console::Find( StringView name )
{
	auto result = cvarList.find( name );
	if ( result == cvarList.end() )
	{
		return nullptr;
	}

	return result->second;
}

// ============================
// Console::GetArguments
// ============================
const adm::Dictionary& Console::GetArguments() const
{
	return arguments;
}

// ============================
// Console::ParseArguments
// ============================
void Console::ParseArguments( int argc, char** argv )
{
	// Skip the 1st argument - that is the path to the .exe
	if ( argc == 1 )
	{
		return;
	}

	for ( int i = 1; i < argc; i++ )
	{
		const char* string = argv[i];
		const char* param = "";

		// Args prefixed with - are switches
		// Prefixed with + are commands
		if ( string[0] == '-' || string[0] == '+' )
		{
			if ( i < argc - 1 )
			{
				param = argv[++i];

				// Switches and commands not always need a parameter
				if ( param[0] == '-' || param[0] == '+' )
				{
					i--;
					param = "";
				}
			}
		}

		// In you go
		arguments.SetCString( string, param );
	}
}

// ============================
// Console::LogLine
// Filters out $rgb colours in a string
// and prints/logs it that way
// ============================
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
			i += 3; // skip the $rgb sequence
			continue; // does i += 1
		}
		// Store the character
		buffer[position] = string[i];
		position++;
	}

	buffer[position] = '\0';

	std::cout << timeString << " | " << buffer << std::endl;
}

// ============================
// Console::Log
// Dissects a string into lines and prints
// them separately with timestamps
// ============================
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
			// TODO: could've been done using memcpy
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

// ============================
// Console::GenerateTimeString
// ============================
char* Console::GenerateTimeString()
{
	// mmm:ss.ssss
	static char buffer[16];
	const float time = core->Time();

	int iTime = time;
	int seconds = time;
	int minutes = seconds / 60;

	seconds = seconds % 60;

	float flSeconds = seconds + (time - iTime);

	sprintf( buffer, "%03i:%06.3f", minutes, flSeconds );
	return buffer;
}
