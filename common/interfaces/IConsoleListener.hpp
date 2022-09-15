
#pragma once

class ICore;
class IConsole;

constexpr size_t MaxConsoleHistoryLines = 2048U;

constexpr const char PrintColorIdentifier = '$';
constexpr const char* PrintRed		= "$r";
constexpr const char* PrintOrange	= "$o";
constexpr const char* PrintYellow	= "$y";
constexpr const char* PrintGreen	= "$g";
constexpr const char* PrintBlue		= "$b";
constexpr const char* PrintPink		= "$p";
constexpr const char* PrintWhite	= "$w";
constexpr const char* PrintGrey		= "$G";

struct ConsoleMessage final
{
	ConsoleMessage() = default;
	ConsoleMessage( const ConsoleMessage& message ) = default;
	ConsoleMessage( ConsoleMessage&& message ) = default;
	ConsoleMessage& operator=( const ConsoleMessage& message ) = default;
	ConsoleMessage& operator=( ConsoleMessage&& message ) = default;

	String 		text{};
	float 		timeSubmitted{};
	DateTime	date{};
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
