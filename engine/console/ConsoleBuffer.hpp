
#pragma once

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

// Console line width is variable, depending on the font scale and screen width

class ConsoleString final
{
public:
	ConsoleString( float time, StringRef textRef )
		: submissionTime(time), text(textRef)
	{

	}

	float submissionTime;
	String text;
};

class ConsoleBuffer final
{
public:
	void Init();
	void Write( StringRef text, float time = 0.0f );
	// Temporarily here until we get the clientside + renderer
	void Display( int offsetFromEnd, class IRenderFrontend* rf = nullptr ) {}

private:
	std::vector<ConsoleString> buffer;
};
