
#pragma once

// Generic app interface. This is as simple as it gets, really
// You can use this to implement a game plugin, first and foremost
// Can also be used to implement tools
class IApplication : public IPlugin
{
public:
	const char* GetInterfaceName() const
	{
		return "IApplication";
	}

	// Called every frame
	virtual void Update() = 0;

	static constexpr const char* Name = "IApplication";
};
