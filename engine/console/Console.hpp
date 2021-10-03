
#pragma once

class IConsole;

class Console final : public IConsole
{
public:
	void Init() override;
	void Shutdown() override;

	void Register( CVarBase* cvar ) override;
	bool Execute( StringRef command, StringRef args ) override;

	CVarBase* Find( StringRef name ) override;

	static inline CVarList EngineCVarList = CVarList();

private:
	CVarList cvarList;
};

// There should be another one like this in the game DLL,
// potentially yet another one in the clientside portion of it
using CVar = CVarTemplate<Console::EngineCVarList>;
