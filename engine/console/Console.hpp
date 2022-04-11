
#pragma once

class IConsole;

#include "ConsoleBuffer.hpp"

class Console final : public IConsole
{
public:
	bool		Init( int argc, char** argv ) override;
	void		Shutdown() override;

	void        Print( const char* string ) override;
	void        DPrint( const char* string, int developerLevel ) override;
	void        Warning( const char* string ) override;
	void        Error( const char* string ) override;

	void		Register( CVarBase* cvar ) override;
	bool		Execute( StringRef command, StringRef args ) override;

	CVarBase*	Find( StringRef name ) override;

	const adm::Dictionary& GetArguments() const override;

	void		Setup( ICore* core )
	{
		this->core = core;
	}

	// Glue for the automatic CVar registration system - look at CVarTemplate
	static inline CVarList EngineCVarList = CVarList();
	static inline IConsole* EngineConsole = nullptr; // I trust that nobody will fiddle with this

private:
	void		ParseArguments( int argc, char** argv );

	void		LogLine( const char* string, const char* timeString );
	void		Log( const char* string, const char* timeString );
	char*		GenerateTimeString();

private:
	ConsoleBuffer buffer;
	CVarList	cvarList;
	adm::Dictionary arguments;
	ICore*		core{ nullptr };
};

// There should be another one like this in the game DLL
using CVar = CVarTemplate<Console::EngineCVarList, Console::EngineConsole>;
