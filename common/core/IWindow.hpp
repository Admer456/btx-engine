
#pragma once

struct WindowCreateDesc
{
	const char* windowName{};
	uint32_t width{};
	uint32_t height{};
	
	// -1 = start at the centre of the screen
	int32_t offsetX{ -1 };
	int32_t offsetY{ -1 };

	bool startFullscreen{ false };
	// Start windowed but maximised
	bool startMaximised{ false };
	// Start windowed and minimised
	bool startMinimised{ false };
	// Can be resized
	bool resizeable{ false };
	// Borderless window. If startMaximised is enabled and this is
	// enabled too, then you can fake fullscreen mode
	bool borderless{ false };
};

struct WindowSurfaceData
{
#if ADM_PLATFORM == ADM_PLATFORM_WINDOWS

	HINSTANCE hInstance{};
	HWND hWindow{};

#elif ADM_PLATFORM == ADM_PLATFORM_LINUX

#if ADM_USE_X11
	Display* display{ nullptr };
	Window window{};
#endif

#if ADM_USE_WAYLAND
	wl_display* display{ nullptr };
	wl_surface* surface{ nullptr };
#endif

#endif
};

enum class WindowVideoFormat
{
	SRGBA8,
	RGBA8,
	SBGRA8,
	BGRA8
};

struct WindowVideoMode
{
	uint32_t width{};
	uint32_t height{};
	uint32_t refreshRate{ 60 };
	WindowVideoFormat format = WindowVideoFormat::SBGRA8;
};

class IWindow
{
public:
	virtual const char* GetWindowName() const = 0;
	virtual WindowSurfaceData GetSurfaceData() const = 0;

	// Full screen functionality

	virtual void SetFullscreen( bool fullscreen ) = 0;
	virtual bool GetFullscreen() const = 0;
	virtual void SetVideoMode( const WindowVideoMode& mode ) = 0;
	virtual WindowVideoMode GetVideoMode() const = 0;
	virtual Vector<WindowVideoMode> GetAvailableVideoModes() const = 0;

	// Windowed functionality

	// Raise and set focus
	virtual void Raise() = 0;

	virtual void Maximise() = 0;
	virtual void Minimise() = 0;
	// Todo: we need a Vec2 type real bad :(
	virtual void Resize( adm::Vec3 size ) = 0;
	virtual void Resize( adm::Vec3 topLeft, adm::Vec3 bottomRight ) = 0;
	virtual bool HasResized() const = 0;

	virtual adm::Vec3 GetSize() const = 0;
	virtual adm::Vec3 GetPosition() const = 0;

	virtual void* GetInternalPointer() const = 0;
};
