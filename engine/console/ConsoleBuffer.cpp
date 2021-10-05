
#include "common/Precompiled.hpp"
#include "Console.hpp"

// ============================
// ConsoleBuffer::Init
// ============================
void ConsoleBuffer::Init()
{
	buffer.clear();
	buffer.reserve( MaxConsoleHistoryLines );
}

// ============================
// ConsoleBuffer::Write
// ============================
void ConsoleBuffer::Write( StringRef text, float time )
{
	if ( buffer.size() >= MaxConsoleHistoryLines )
	{
		buffer.erase( buffer.begin() );
	}

	buffer.emplace_back( time, text );
}
