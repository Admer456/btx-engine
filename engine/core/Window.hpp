// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

class Window final : public IWindow
{
public:
	Window( SDL_Window* sdlWindow );

	const char* GetWindowName() const override;
	WindowSurfaceData GetSurfaceData() const override;

	void SetFullscreen( bool fullscreen ) override;
	bool GetFullscreen() const override;
	void SetVideoMode( const WindowVideoMode& mode ) override;
	WindowVideoMode GetVideoMode() const override;
	Vector<WindowVideoMode> GetAvailableVideoModes() const override;

	void Raise() override;

	void Maximise() override;
	void Minimise() override;
	void Resize( adm::Vec2 size ) override;
	void Resize( adm::Vec2 topLeft, adm::Vec2 bottomRight ) override;
	bool HasResized() const override;
	
	adm::Vec2 GetSize() const override;
	adm::Vec2 GetPosition() const override;
	
	void* GetInternalPointer() const override;

	void ResetResizedState();

private:
	int GetDisplayIndex() const;

private:
	SDL_Window* window{ nullptr };
	bool gotResized{ false };
	bool isFullscreen{ false };
};
