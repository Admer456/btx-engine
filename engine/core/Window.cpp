// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "common/Precompiled.hpp"

#include "SDL_video.h"
#include "SDL_syswm.h"

#include "Window.hpp"
#include "VideoFormat.hpp"

static VideoFormatBinding WindowFormats
{
	{ WindowVideoFormat::SBGRA8, SDL_PIXELFORMAT_BGRA8888 },
	{ WindowVideoFormat::BGRA8, SDL_PIXELFORMAT_BGRA8888 },
	{ WindowVideoFormat::SRGBA8, SDL_PIXELFORMAT_RGBA8888 },
	{ WindowVideoFormat::RGBA8, SDL_PIXELFORMAT_RGBA8888 }
};

// ============================
// Window::ctor
// ============================
Window::Window( SDL_Window* sdlWindow )
	: window( sdlWindow )
{
	isFullscreen = SDL_GetWindowFlags( sdlWindow ) | SDL_WINDOW_FULLSCREEN;
}

// ============================
// Window::GetWindowName
// ============================
const char* Window::GetWindowName() const
{
	return SDL_GetWindowTitle( window );
}

// ============================
// Window::GetSurfaceData
// ============================
WindowSurfaceData Window::GetSurfaceData() const
{
	WindowSurfaceData data;

	SDL_SysWMinfo info;
	SDL_VERSION( &info.version );
	SDL_GetWindowWMInfo( window, &info );

#if ADM_PLATFORM == PLATFORM_WINDOWS
	data.hInstance = info.info.win.hinstance;
	data.hWindow = info.info.win.window;
#elif ADM_PLATFORM == PLATFORM_LINUX
	data.display = info.info.x11.display;
	data.window = info.info.x11.window;
#endif

	return data;
}

// ============================
// Window::SetFullscreen
// ============================
void Window::SetFullscreen( bool fullscreen )
{
	isFullscreen = fullscreen;

	uint32_t sdlFlags = isFullscreen ? SDL_WINDOW_FULLSCREEN : 0;
	SDL_SetWindowFullscreen( window, sdlFlags );
}

// ============================
// Window::GetFullscreen
// ============================
bool Window::GetFullscreen() const
{
	return isFullscreen;
}

// ============================
// Window::SetVideoMode
// ============================
void Window::SetVideoMode( const WindowVideoMode& mode )
{
	SDL_DisplayMode dm, closest;
	dm.w = mode.width;
	dm.h = mode.height;
	dm.refresh_rate = mode.refreshRate;
	dm.format = WindowFormats.Find( mode.format );
	dm.driverdata = nullptr;

	if ( nullptr == SDL_GetClosestDisplayMode( GetDisplayIndex(), &dm, &closest ) )
	{
		return;
	}

	SDL_SetWindowDisplayMode( window, &closest );
}

// ============================
// Window::GetVideoMode
// ============================
WindowVideoMode Window::GetVideoMode() const
{
	WindowVideoMode mode;
	SDL_DisplayMode dm;
	if ( SDL_GetWindowDisplayMode( window, &dm ) != 0 )
	{
		return mode;
	}

	mode.width = dm.w;
	mode.height = dm.h;
	mode.refreshRate = dm.refresh_rate;
	mode.format = WindowFormats.Find( SDL_PixelFormatEnum( dm.format ) );

	return mode;
}

// ============================
// Window::GetAvailableVideoModes
// ============================
Vector<WindowVideoMode> Window::GetAvailableVideoModes() const
{
	Vector<WindowVideoMode> videoModes{};
	const int displayIndex = GetDisplayIndex();

	const auto sdlDisplayModeToVideoMode = []( SDL_DisplayMode dm ) -> WindowVideoMode
	{
		WindowVideoMode mode;

		mode.width = dm.w;
		mode.height = dm.h;
		mode.refreshRate = dm.refresh_rate;
		mode.format = WindowFormats.Find( SDL_PixelFormatEnum( dm.format ) );

		return mode;
	};

	int numModes = SDL_GetNumDisplayModes( displayIndex );
	if ( numModes > 1 )
	{
		videoModes.reserve( numModes );
		SDL_DisplayMode dm;

		for ( int i = 0; i < numModes; i++ )
		{
			if ( SDL_GetDisplayMode( displayIndex, i, &dm ) == 0 )
			{
				videoModes.push_back( sdlDisplayModeToVideoMode( dm ) );
			}
		}

		videoModes.shrink_to_fit();
	}

	return videoModes;
}

// ============================
// Window::Raise
// ============================
void Window::Raise()
{
	SDL_RaiseWindow( window );
}

// ============================
// Window::Maximise
// ============================
void Window::Maximise()
{
	SDL_MaximizeWindow( window );
	gotResized = true;
}

// ============================
// Window::Minimise
// ============================
void Window::Minimise()
{
	SDL_MinimizeWindow( window );
}

// ============================
// Window::Resize
// ============================
void Window::Resize( adm::Vec2 size )
{
	SDL_SetWindowSize( window, int( size.x ), int( size.y ) );
	gotResized = true;
}

// ============================
// Window::Resize
// ============================
void Window::Resize( adm::Vec2 topLeft, adm::Vec2 bottomRight )
{
	if ( topLeft.x > bottomRight.x )
	{
		std::swap( topLeft, bottomRight );
	}

	adm::Vec2 size = bottomRight - topLeft;
	size.y = std::abs( size.y );

	return Resize( size );
}

// ============================
// Window::HasResized
// ============================
bool Window::HasResized() const
{
	return gotResized;
}

// ============================
// Window::GetSize
// ============================
adm::Vec2 Window::GetSize() const
{
	int x, y;
	SDL_GetWindowSize( window, &x, &y );

	return { float( x ), float( y ) };
}

// ============================
// Window::GetPosition
// ============================
adm::Vec2 Window::GetPosition() const
{
	int x, y;
	SDL_GetWindowPosition( window, &x, &y );

	return { float( x ), float( y ) };
}

// ============================
// Window::GetInternalPointer
// ============================
void* Window::GetInternalPointer() const
{
	return window;
}

// ============================
// Window::ResetResizedState
// ============================
void Window::ResetResizedState()
{
	gotResized = false;
}

// ============================
// Window::GetDisplayIndex
// ============================
int Window::GetDisplayIndex() const
{
	return SDL_GetWindowDisplayIndex( window );
}
