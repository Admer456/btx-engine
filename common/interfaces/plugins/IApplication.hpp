
#pragma once

// Generic app interface. This is as simple as it gets, really
// You can use this to implement a game plugin, first and foremost
// Can also be used to implement tools
class IApplication : public IPlugin
{
public:
	DeclarePluginInterface( IApplication );

	// Called after the engine has initialised all subsystems
	virtual void Start() = 0;

	// Called every frame
	virtual void Update() = 0;
};
