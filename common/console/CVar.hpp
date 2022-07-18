
#pragma once

using ConsoleCommandArgs = Vector<StringView>;
using ConsoleCommandFn = bool( const ConsoleCommandArgs& args );

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
// - TODO: autocomplete, limits etc.?
// 
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
	int			GetInt() const;
	float		GetFloat() const;
	bool		GetBool() const;
	const char*	GetCString() const;
	StringView	GetString() const;

	// Setters
	void		SetInt( const int& value );
	void		SetFloat( const float& value );
	void		SetBool( const bool& value );
	void		SetCString( const char* value );
	void		SetString( StringView value );

protected:
	bool		Execute( const Vector<StringView>& args, IConsole* console );

protected:
	bool		isCommand{ false };
	ConsoleCommandFn* conCommand{ nullptr };
	String		varName;
	String		varValue;
	String		varDescription;
	uint16_t	varFlags{ 0 };
};

using CVarList = Vector<CVarBase*>;

// CVars are initialised statically, and registered right in the constructor. We do the template stuff here
// to allow the CVars to be registered the exact same way across different modules. The only downside is that each
// module will have to have its own alias for CVars:
// 
// using CVar = CVarTemplate<cvarList, gConsole>;
// 
// In this instance, there is only one for the engine, and one for the game DLL.

// CVar template for automatic registration in the console system
// The template only provides automatic registration functionality. For the rest, look at CVarBase
template<CVarList& staticCVarList, IConsole*& console>
class CVarTemplate : public CVarBase
{
public:
	CVarTemplate( const char* name, const char* defaultValue = nullptr, uint16_t flags = 0, const char* description = "" )
		: CVarBase( name, defaultValue, flags, description )
	{
		if ( !RegisteredAllStatics )
		{
			staticCVarList.push_back( this );
		}
		else
		{
			console->Register( this );
		}
	}

	CVarTemplate( const char* name, ConsoleCommandFn* function, const char* description = "" )
		: CVarBase( name, function, description )
	{
		if ( !RegisteredAllStatics )
		{
			staticCVarList.push_back( this );
		}
		else
		{
			console->Register( this );
		}
	}

	~CVarTemplate()
	{
		if ( nullptr == console )
		{
			return;
		}

		console->Unregister( this );
	}

	// my_command my_argument my_argument2
	// If my_command is a variable, then args is trimmed to just "my_argument"
	// Otherwise, args is "my_argument my_argument2", and you're free to parse 
	// it in any way you want
	int Execute( StringView args )
	{
		return Execute( args, console );
	}

	// Init-time CVars that were statically declared
	static void RegisterAll()
	{
		for ( auto& cvar : staticCVarList )
		{
			console->Register( cvar );
		}

		RegisteredAllStatics = true;
	}

	// Unregister static CVars
	// CVars that are members of classes should get unregistered automatically there
	static void UnregisterAll()
	{
		for ( auto& cvar : staticCVarList )
		{
			console->Unregister( cvar );
		}

		RegisteredAllStatics = false;
	}

	inline static bool RegisteredAllStatics = false;
};

// In game DLL:
// using CVar = CVarTemplate<GameCVarList, Console>
// 
// In engine:
// using CVar = CVarTemplate<Console::EngineCVarList, Console::EngineConsole>
