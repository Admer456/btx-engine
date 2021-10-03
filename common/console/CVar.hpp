
#pragma once

using ConsoleCommandFn = bool( StringRef args );

enum CVarFlags
{
	CVar_Saved      = 1 << 1, // saved to a file, so it's the same in all sessions
	CVar_ReadOnly   = 1 << 2, // cannot be modified
	CVar_Replicated = 1 << 3, // replicated from server to clients
};

// CVars are defined by the following:
// - a name
// - a default value
// - flags (replicated, saved, server-only, read-only etc.)
// - a description
// Console commands are also CVars but with a special purpose, defined by:
// - a name
// - a function pointer
// - a description

class CVarBase
{
public: // Construction site
	CVarBase( const char* name, const char* defaultValue, uint16_t flags, const char* description );
	CVarBase( const char* name, ConsoleCommandFn* function, const char* description );

	// So that the console system can easily access stuff
	// here. This does nothing in the game library tho'
	friend class Console;

	// Getters
	int         GetInt() const;
	float       GetFloat() const;
	bool        GetBool() const;
	const char* GetCString() const;
	StringRef   GetString() const;

	// Setters
	void		SetInt( const int& value );
	void		SetFloat( const float& value );
	void		SetBool( const bool& value );
	void		SetCString( const char* value );
	void		SetString( StringRef value );

	// my_command my_argument my_argument2
	// If my_command is a variable, then args is trimmed to just "my_argument"
	// Otherwise, args is "my_argument my_argument2", and you're free to parse 
	// it in any way you want
	bool		Execute( StringRef args );

protected:
	bool        isCommand{ false };
	ConsoleCommandFn* conCommand{ nullptr };
	String      varName;
	String      varValue;
	String      varDescription;
	uint16_t    varFlags{ 0 };
};

using CVarList = std::vector<CVarBase*>;

// ------------------------------------------------------------------------
// This is a little ugly, but, for the static CVars to work, we need this 
// thing, where each module defines its own gConsole. It's fine here, 
// because it'll only be defined twice: in the engine and in the game DLL
// ------------------------------------------------------------------------
// It is defined in these places at the moment:
// engine/console/Console.cpp
// game/shared/Console.cpp (not actually there yet)
extern IConsole* gConsole;

template<CVarList& staticCVarList>
class CVarTemplate : public CVarBase
{
public:
	CVarTemplate( const char* name, const char* defaultValue, uint16_t flags, const char* description )
		: CVarBase( name, defaultValue, flags, description )
	{
		if ( !RegisteredAllStatics )
		{
			staticCVarList.push_back( this );
		}
		else
		{
			gConsole->Register( this );
		}
	}

	CVarTemplate( const char* name, ConsoleCommandFn* function, const char* description )
		: CVarBase( name, function, description )
	{
		if ( !RegisteredAllStatics )
		{
			staticCVarList.push_back( this );
		}
		else
		{
			gConsole->Register( this );
		}
	}

	// Init-time CVars that were statically declared
	static void RegisterAll()
	{
		for ( auto& cvar : staticCVarList )
		{
			gConsole->Register( cvar );
		}

		RegisteredAllStatics = true;
	}

	inline static bool RegisteredAllStatics = false;
};

// In game DLL:
// using CVar = CVarTemplate<clientCVarList> (client)
// using CVar = CVarTemplate<serverCVarList> (server)
// 
// In engine:
// using CVar = CVarTemplate<Console::EngineCVarList>
