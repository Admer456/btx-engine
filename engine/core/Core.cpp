
#include "SDL.h"

#include "common/Precompiled.hpp"
#include "../console/Console.hpp"
#include "Core.hpp"
#include "Window.hpp"

CVar developer( "developer", "0", 0, "Developer mode" );

// ============================
// Core::Init
// ============================
bool Core::Init()
{
	systemTimer.Reset();

	int sdlResult = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );

	// If this somehow happens, congrats
	if ( sdlResult < 0 )
	{
		return false;
	}

	return true;
}

// ============================
// Core::Shutdown
// There's nothing to do here at the moment,
// so shutting down the core would be the safest thing in this codebase
// ============================
void Core::Shutdown()
{
	for ( auto*& window : windows )
	{
		DestroyWindow( window );
	}

	SDL_Quit();
}

// ============================
// Core::DevLevel
// ============================
int Core::DevLevel() const
{
	return developer.GetInt();
}

// ============================
// Core::Time
// ============================
float Core::Time() const
{
	return systemTimer.GetElapsed( adm::TimeUnits::Seconds );
}

// ============================
// Core::TimeMilliseconds
// ============================
float Core::TimeMilliseconds() const
{
	return systemTimer.GetElapsed( adm::TimeUnits::Milliseconds );
}

// ============================
// Core::DeltaTime
// ============================
float Core::DeltaTime() const
{
	return deltaTime;
}

// ============================
// Core::SetDeltaTime
// ============================
void Core::SetDeltaTime( const float& newDeltaTime )
{
	deltaTime = newDeltaTime;
}

// ============================
// Core::IsHeadless
// ============================
bool Core::IsHeadless() const
{
	return isHeadless;
}

// ============================
// Core::SetHeadless
// ============================
void Core::SetHeadless( bool headless )
{
	isHeadless = headless;
}

// ============================
// Core::CreateWindow
// ============================
IWindow* Core::CreateWindow( const WindowCreateDesc& desc )
{
	int offsetX = desc.offsetX == -1 ? SDL_WINDOWPOS_CENTERED : desc.offsetX;
	int offsetY = desc.offsetY == -1 ? SDL_WINDOWPOS_CENTERED : desc.offsetY;
	int flags = desc.resizeable ? SDL_WINDOW_RESIZABLE : 0;
	
	// Fullscreen modes
	if ( desc.startFullscreen )
	{
		if ( desc.borderless )
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
	}
	// Windowed modes
	else
	{
		if ( desc.startMaximised )
		{
			flags |= SDL_WINDOW_MAXIMIZED;
		}
		else if ( desc.startMinimised )
		{
			flags |= SDL_WINDOW_MINIMIZED;
		}
	}

	SDL_Window* sdlWindow = SDL_CreateWindow( desc.windowName, offsetX, offsetY, desc.width, desc.height, flags );
	Window* windowObject = new Window( sdlWindow );

	windows.push_back( windowObject );
	return windows.back();
}

// ============================
// Core::DestroyWindow
// ============================
void Core::DestroyWindow( IWindow* window )
{
	for ( auto it = windows.begin(); it != windows.end(); it++ )
	{
		if ( *it == window )
		{
			SDL_DestroyWindow( static_cast<SDL_Window*>( window->GetInternalPointer() ) );
			windows.erase( it );
			return;
		}
	}
}

// ============================
// Core::GetWindows
// ============================
Vector<IWindow*>& Core::GetWindows()
{
	return windows;
}

// ============================
// Core::GetErrorMessage
// ============================
const char* Core::GetErrorMessage() const
{
	return SDL_GetError();
}
