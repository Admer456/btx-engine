// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"
#include "Console.hpp"
#include <iostream>

// ============================
// ConsoleListenerBasic
// ============================
class ConsoleListenerBasic final : public ConsoleListenerBase
{
public:
	void Init( ICore* core, IConsole* console ) override;
	void Shutdown() override;

	void OnLog( const ConsoleMessage& message ) override;
	void OnUpdate() override;

	const char* GetName() override
	{
		return "Standard console output";
	}

private:
	const char* GenerateTimeString( float time );
};

// ============================
// ConsoleListenerBasic::Init
// ============================
void ConsoleListenerBasic::Init( ICore* core, IConsole* console )
{
	ConsoleListenerBase::Init( core, console );
}

// ============================
// ConsoleListenerBasic::Shutdown
// ============================
void ConsoleListenerBasic::Shutdown()
{

}

// ============================
// ConsoleListenerBasic::OnLog
// ============================
void ConsoleListenerBasic::OnLog( const ConsoleMessage& message )
{
	const char* string = message.text.c_str();
	char buffer[256];
	size_t position = 0U;
	size_t max = std::min( 256ULL, message.text.size() );

	// Too lazy to implement colours at the moment
	for ( size_t i = 0U; i < max; i++ )
	{
		// Initiate the skipping
		if ( string[i] == PrintColorIdentifier )
		{
			i++; // skip the $-colour sequence
			continue; // does i += 1
		}
		// Store the character
		buffer[position] = string[i];
		position++;
	}

	buffer[position] = '\0';

	std::cout << GenerateTimeString( message.timeSubmitted ) << " | " << buffer << std::endl;
}

// ============================
// ConsoleListenerBasic::OnUpdate
// ============================
void ConsoleListenerBasic::OnUpdate()
{

}

// ============================
// ConsoleListenerBasic::GenerateTimeString
// ============================
const char* ConsoleListenerBasic::GenerateTimeString( float time )
{
	// mmm:ss.ssss
	static char buffer[16];

	const int iTime = time;
	const int seconds = int( time ) % 60;
	const int minutes = iTime / 60;

	const float flSeconds = seconds + (time - iTime);

	sprintf( buffer, "%03i:%06.3f ", minutes, flSeconds );
	return buffer;
}

// ============================
// Console::CreateListenerBasic
// ============================
IConsoleListener* Console::CreateListenerBasic()
{
	return new ConsoleListenerBasic();
}
