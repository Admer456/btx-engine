
#pragma once

class CVarBase;

class IConsole
{
public:
	virtual bool	Init( int argc, char** argv ) = 0;
	virtual void	Shutdown() = 0;

	// Use adm::format() if you wanna format it
	virtual void	Print( const char* string ) = 0;
	virtual void	DPrint( const char* string, int developerLevel ) = 0;
	virtual void	Warning( const char* string ) = 0;
	virtual void	Error( const char* string ) = 0;

	// Registers a CVar internally so it can be executed upon
	// Usually, you might not need to use this at all
	virtual void	Register( CVarBase* cvar ) = 0;

	// Unregisters a CVar
	virtual void	Unregister( CVarBase* cvar ) = 0;

	// Executes a console command
	// @param command: The command or CVar name
	// @param args: Space-separated arguments
	virtual bool	Execute( StringView command, StringView args ) = 0;

	// Executes a console command
	// @param command: The command or CVar name
	// @param args: A list of arguments
	virtual bool	Execute( StringView command, const Vector<StringView>& args ) = 0;

	virtual CVarBase* Find( StringView name ) = 0;

	virtual const adm::Dictionary& GetArguments() const = 0;
};
