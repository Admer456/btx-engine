
#include "common/Precompiled.hpp"
#include "Console.hpp"

void ConsoleBuffer::Init()
{
	buffer.clear();
	buffer.reserve( MaxConsoleHistoryLines );
}

void ConsoleBuffer::Write( StringRef text, float time )
{
	if ( buffer.size() >= MaxConsoleHistoryLines )
	{
		buffer.erase( buffer.begin() );
	}

	buffer.emplace_back( time, text );
}
