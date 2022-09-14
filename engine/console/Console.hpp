
#pragma once

class IConsole;

using CVarList = Vector<CVarBase*>;

class Console final : public IConsole
{
public:
	bool		Init( int argc, char** argv ) override;
	void		Shutdown() override;

	// Called by the engine to update the listeners
	void		Update();

	void		AddListener( IConsoleListener* listener ) override;

	void		Print( const char* string ) override;
	void		DPrint( const char* string, int developerLevel ) override;
	void		Warning( const char* string ) override;
	void		Error( const char* string ) override;

	void		Register( CVarBase* cvar ) override;
	void		Unregister( CVarBase* cvar ) override;

	bool		Execute( StringView command, StringView args ) override;
	bool		Execute( StringView command, const ConsoleCommandArgs& args ) override;

	const CVarBase* Find( StringView name ) const override;
	Vector<const CVarBase*> Search( StringView nameFragment ) const override;

	const adm::Dictionary& GetArguments() const override;

	void		Setup( ICore* core )
	{
		this->core = core;
	}

	// Glue for the automatic CVar registration system - look at CVarTemplate
	static inline IConsole* EngineConsole = nullptr; // I trust that nobody will fiddle with this

private:
	void		ParseArguments( int argc, char** argv );

	void		LogLine( const ConsoleMessage& message );
	void		Log( const ConsoleMessage& message );

	// ConsoleListenerBasic.cpp
	// Non-interactive terminal output
	static IConsoleListener* CreateListenerBasic();
	// ConsoleListenerInteractive.cpp
	// Interactive terminal UI console
	static IConsoleListener* CreateListenerInteractive();
	// ConsoleListenerFileOut.cpp
	// File logger for this console
	static IConsoleListener* CreateListenerFileOut();

private:
	Vector<IConsoleListener*> consoleListeners;
	CVarList	cvarList;
	Dictionary	arguments;
	ICore*		core{ nullptr };
};

// There should be another one like this in the game DLL
using CVar = CVarTemplate<Console::EngineConsole>;
