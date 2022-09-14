
#pragma once

class ICore;
class IConsole;

constexpr size_t MaxConsoleHistoryLines = 2048U;

constexpr const char PrintColorIdentifier = '$';
constexpr const char* PrintRed		= "$d44";
constexpr const char* PrintOrange	= "$e73";
constexpr const char* PrintYellow	= "$dc2";
constexpr const char* PrintGreen	= "$6d5";
constexpr const char* PrintBlue		= "$4ae";
constexpr const char* PrintPink		= "$e8d";
constexpr const char* PrintWhite	= "$eee";
constexpr const char* PrintGrey		= "$aaa";

struct ConsoleMessage
{
	String 		text;
	float 		timeSubmitted;
	DateTime	date;
};

// The idea behind console listeners is that they react to console events but also
// provide a frontend to the user. In theory you could write an in-game console listener
// to implement a Quake-style in-game developer console, however here I am just doing
// a TUI and a file logger
class IConsoleListener
{
public:
	virtual void Init( ICore* core, IConsole* console ) = 0;
	virtual void Shutdown() = 0;

	// Called every time the console logs something
	virtual void OnLog( const ConsoleMessage& message ) = 0;
	// Called every frame, so the listener can do state updates, animations etc.
	virtual void OnUpdate() = 0;

	virtual const char* GetName() = 0;
};

class ConsoleListenerBase : public IConsoleListener
{
public:
	virtual void Init( ICore* core, IConsole* console ) override
	{
		this->core = core;
		this->console = console;
	}

protected:
	ICore* core{};
	IConsole* console{};
};
