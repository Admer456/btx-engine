
#pragma once

class IConsole;

#include "ConsoleBuffer.hpp"

class Console final : public IConsole
{
public:
	void		Init() override;
	void		Shutdown() override;

	void        Print( const char* string ) override;
	void        DPrint( const char* string, int developerLevel ) override;
	void        Warning( const char* string ) override;
	void        Error( const char* string ) override;

	void		Register( CVarBase* cvar ) override;
	bool		Execute( StringRef command, StringRef args ) override;

	CVarBase*	Find( StringRef name ) override;

	void		Setup( ICore* core )
	{
		this->core = core;
	}

	static inline CVarList EngineCVarList = CVarList();

private:
	void		Log( const char* string );

private:
	ConsoleBuffer buffer;
	CVarList	cvarList;
	ICore*		core{ nullptr };
};

// There should be another one like this in the game DLL,
// potentially yet another one in the clientside portion of it
using CVar = CVarTemplate<Console::EngineCVarList>;
