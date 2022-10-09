// SPDX-FileCopyrightText: 2021-2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

class ICore;

class Core : public ICore
{
public:
	bool		Init() override;
	void		Shutdown() override;

	int			DevLevel() const override;

	// Returns the current time since the app started running
	float		Time() const override;
	float		TimeMilliseconds() const override;
	float		DeltaTime() const override;
	// For the engine to set delta time
	void		SetDeltaTime( const float& newDeltaTime );
	
	bool		IsHeadless() const override;
	// For the engine to set headless state
	void		SetHeadless( bool headless );

	IWindow*	CreateWindow( const WindowCreateDesc& desc ) override;
	void		DestroyWindow( IWindow* window ) override;
	Vector<IWindow*>& GetWindows() override;

	// For the engine to get the SDL2 error message, in case of failure
	const char* GetErrorMessage() const;

private:
	bool		isHeadless{ false };
	Vector<IWindow*> windows{};
	adm::Timer	systemTimer;
	float		deltaTime{ 0.0f };
};
