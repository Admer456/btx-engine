
#pragma once

struct WindowCreateDesc;
class IWindow;

// System abstraction layer
class ICore
{
public:
	virtual bool	Init() = 0;
	virtual void	Shutdown() = 0;

	// Developer level
	virtual int		DevLevel() const = 0;

	// Returns the current time since the app started running
	virtual float	Time() const = 0;
	virtual float	TimeMilliseconds() const = 0;
	// Engine's delta time, from the start to the end of the last frame
	virtual float	DeltaTime() const = 0;

	// Is this engine instance headless? (i.e. has no window, rendering & audio)
	virtual bool	IsHeadless() const = 0;

	// Windowing
	virtual IWindow* CreateWindow( const WindowCreateDesc& desc ) = 0;
	virtual void	DestroyWindow( IWindow* window ) = 0;
	virtual Vector<IWindow*>& GetWindows() = 0;
};
