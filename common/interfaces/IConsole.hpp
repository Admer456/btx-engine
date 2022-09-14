
#pragma once

class CVarBase;
class IConsoleListener;

class IConsole
{
public:
	virtual bool	Init( int argc, char** argv ) = 0;
	virtual void	Shutdown() = 0;

	// Look at IConsoleListener.hpp
	virtual void	AddListener( IConsoleListener* listener ) = 0;

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
	virtual bool	Execute( StringView command, const ConsoleCommandArgs& args ) = 0;

	// Finds a CVar by name
	virtual const CVarBase* Find( StringView name ) const = 0;

	// @returns A list of CVars
	virtual Vector<const CVarBase*> Search( StringView nameFragment ) const = 0;

	virtual const adm::Dictionary& GetArguments() const = 0;
};
