
#pragma once

class IConsole;

using ConsoleCommandArgs = Vector<String>;
using ConsoleCommandFn = bool( const ConsoleCommandArgs& args );

struct CVarFlags
{
	enum Enum : uint16_t
	{
		// Saved to a file, so it's the same in all sessions
		Saved = 1 << 0,
		// Cannot be modified
		ReadOnly = 1 << 1,
		// Replicated from server to clients
		Replicated = 1 << 2,
		// Appears on the serverside
		Server = 1 << 3,
		// Appears on the clientside
		Client = 1 << 4
	};
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

	// @returns Whether or not this is a command
	bool		IsCommand() const;

	// Getters
	StringView	GetName() const;
	uint16_t	GetFlags() const;
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
	bool		Execute( const ConsoleCommandArgs& args, IConsole* console );

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
// @param Slot: Separates CVar lists
template<IConsole*& console, int Slot = 0>
class CVarTemplate : public CVarBase
{
public:
	CVarTemplate( const char* name, const char* defaultValue = nullptr, uint16_t flags = 0, const char* description = "" )
		: CVarBase( name, defaultValue, flags, description )
	{
		if ( !RegisteredAllStatics )
		{
			StaticCVarList.push_back( this );
		}
		else
		{
			console->Register( this );
			isRegistered = true;
		}
		
	}

	CVarTemplate( const char* name, ConsoleCommandFn* function, const char* description = "" )
		: CVarBase( name, function, description )
	{
		if ( !RegisteredAllStatics )
		{
			StaticCVarList.push_back( this );
		}
		else
		{
			console->Register( this );
			isRegistered = true;
		}
		
	}

	~CVarTemplate()
	{
		if ( nullptr == console || !isRegistered )
		{
			return;
		}

		console->Unregister( this );
		isRegistered = false;
	}

	// my_command my_argument my_argument2
	// If my_command is a variable, then args is trimmed to just "my_argument"
	// Otherwise, args is "my_argument my_argument2", and you're free to parse 
	// it in any way you want
	bool Execute( const Vector<StringView>& args )
	{
		return Execute( args, console );
	}

	// Init-time CVars that were statically declared
	static void RegisterAll()
	{
		for ( auto& cvar : StaticCVarList )
		{
			console->Register( cvar );
			static_cast<CVarTemplate*>(cvar)->isRegistered = true;
		}

		RegisteredAllStatics = true;
	}

	// Unregister static CVars
	// CVars that are members of classes should get unregistered automatically there
	static void UnregisterAll()
	{
		for ( auto& cvar : StaticCVarList )
		{
			console->Unregister( cvar );
			static_cast<CVarTemplate*>(cvar)->isRegistered = false;
		}

		RegisteredAllStatics = false;
	}

	inline static CVarList StaticCVarList = CVarList();
	inline static bool RegisteredAllStatics = false;

private:
	bool isRegistered = false;
};

// In game DLL:
// using CVar = CVarTemplate<Console>
// 
// In engine:
// using CVar = CVarTemplate<Console::EngineConsole>
