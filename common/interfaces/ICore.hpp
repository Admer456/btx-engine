
#pragma once

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

	// Is this a dedicated server?
	// Usually, when this is true, the rendersystem, audiosystem
	// and inputsystem won't be invoked
	virtual bool	IsDedicatedServer() const = 0;
	// Is this instance a server host? (can check for singleplayer with this)
	// @returns false if just a client
	virtual bool	IsHost() const = 0;
};
