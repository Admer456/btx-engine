
#include "common/Precompiled.hpp"
#include "Console.hpp"

// ============================
// Console::Init
// Initialises engine CVars, game CVars are initialised separately
// ============================
bool Console::Init( int argc, char** argv )
{
	ParseArguments( argc, argv );

	EngineConsole = this;
	CVar::RegisterAll();

	cvarList.reserve( 1024U );

	Print( "Scanning for console listeners..." );
	String consoleListenerArguments = arguments.GetCString( "-console_listener", "" );

	// The incoming string is expected to be in the format of alpha,beta,gamma
	// So here we just split it up into tokens and add listeners based on that.
	if ( !consoleListenerArguments.empty() )
	{
		// Some listeners are incompatible with each other as they will
		// fight for the console, such as Basic and TUI
		bool alreadyDoingBasicOrInteractive = false;
		bool alreadyAddedFileout = false;

		Lexer lex( consoleListenerArguments );
		lex.SetDelimiters( "," );

		while ( !lex.IsEndOfFile() )
		{
			String token = lex.Next();

			if ( token == "basic" && !alreadyDoingBasicOrInteractive )
			{
				AddListener( CreateListenerBasic() );
				alreadyDoingBasicOrInteractive = true;
			}
			else if ( token == "interactive" && !alreadyDoingBasicOrInteractive )
			{
				AddListener( CreateListenerInteractive() );
				alreadyDoingBasicOrInteractive = true;
			}
			else if ( token == "fileout" && !alreadyAddedFileout )
			{
				AddListener( CreateListenerFileOut() );
				alreadyAddedFileout = true;
			}
			else
			{
				Warning( adm::format( "  * unknown listener option '%s'", token.c_str() ) );
			}
		}
	}
	// By default, only the basic listener will be there
	else
	{
		AddListener( CreateListenerBasic() );
	}

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

	for ( auto*& listener : consoleListeners )
	{
		listener->Shutdown();
		delete listener;
		listener = nullptr;
	}
	consoleListeners.clear();
}

// ============================
// Console::Update
// ============================
void Console::Update()
{
	for ( auto* listener : consoleListeners )
	{
		listener->OnUpdate();
	}
}

// ============================
// Console::AddListener
// ============================
void Console::AddListener( IConsoleListener* listener )
{
	if ( nullptr == listener )
	{
		Warning( "Console::AddListener: listener was null" );
		return;
	}

	listener->Init( core, this );
	consoleListeners.push_back( listener );

	Print( adm::format( "%sConsole: added listener '%s'", PrintGreen, listener->GetName() ) );
}

// ============================
// Console::Print
// ============================
void Console::Print( const char* string )
{
	ConsoleMessage message;
	message.text = string;
	message.timeSubmitted = core->Time();
	message.date = DateTime::Now();

	Log( message );
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
	do
	{
		String token = lex.Next();
		if ( token.empty() )
		{
			break;
		}

		commandArgs.push_back( token );
	} while ( !lex.IsEndOfFile() );

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
bool Console::Execute( StringView command, const ConsoleCommandArgs& args )
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
CVarBase* Console::Find( StringView name ) const
{
	auto result = cvarList.find( name );
	if ( result == cvarList.end() )
	{
		return nullptr;
	}

	return result->second;
}

// ============================
// Console::Search
// ============================
Vector<CVarBase*> Console::Search( StringView nameFragment ) const
{
	Vector<CVarBase*> cvars{};
	for ( auto& cvarPair : cvarList )
	{
		if ( cvarPair.first.find( nameFragment ) != String::npos )
		{
			cvars.push_back( cvarPair.second );
		}
	}
	return cvars;
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

				// If the next token starts with -/+, then this is parameterless
				// Switches and commands not always need a parameter
				if ( param[0] == '-' || param[0] == '+' )
				{
					i--;
					param = "1"; // so it can be evaluated as a bool
				}
			}
		}

		// In you go
		arguments.SetCString( string, param );
	}
}

// ============================
// Console::LogLine
// ============================
void Console::LogLine( const ConsoleMessage& message )
{
	// The string still contains the colour codes,
	// it is up to the listener to interpret those
	for ( auto*& listener : consoleListeners )
	{
		listener->OnLog( message );
	}
}

// ============================
// Console::Log
// Dissects a string into lines and prints
// them separately with timestamps
// ============================
void Console::Log( const ConsoleMessage& message )
{
	const char* string = message.text.c_str();
	bool hasNewline = message.text.find( '\n' ) != String::npos;

	// Save us the trouble
	if ( !hasNewline )
	{
		return LogLine( message );
	}

	size_t start = 0;
	size_t end = 0;

	char buffer[256];
	size_t max = std::min( 256ULL, std::strlen( string ) );

	// Divide the string by newlines, so for example:
	// abc\ndef becomes:
	// abc
	// def
	ConsoleMessage lineMessage = message;
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

			lineMessage.text = buffer;
			LogLine( lineMessage );
		}
	}
}
