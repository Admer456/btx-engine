
#pragma once

// Shared systems
class ICore;            // sysinfo etc.
class IAnimation;       // animation
class ICollision;       // collision
class IConsole;         // cvars etc.
class IFileSystem;      // files, dirs
class IMaterialManager; // materials
class IModelManager;    // models
class INetwork;         // networking
class IPhysics;         // physics
class IPluginSystem;    // plugins
// Clientside-only systems
class IAudio;           // sound
class IInput;           // input
class IRenderSystem;    // rendering

// The game imports this stuff from the engine
// The engine does not provide any type of save/load system, AI
// system, level/scene system, none of that, it is completely up to
// you to do it in whatever way you want
// 
// In fact, you are free to override any of these systems here in your
// game, like the physics or networking system, if for whatever reason
// you cannot modify the engine itself
struct EngineAPI
{
	int engineVersion{ 0 };

	// Shared stuff
	// Both the server & client game use these

	// System info, timing...
	ICore* core{ nullptr };
	// Animation blending, playback...
	IAnimation* animation{ nullptr };
	// Raycasts, object-to-object tests...
	ICollision* collision{ nullptr };
	// Printing, console vars, console commands...
	IConsole* console{ nullptr };
	// Files, directories
	IFileSystem* fileSystem{ nullptr };
	// Textures, surface properties
	IMaterialManager* materialManager{ nullptr };
	// Model mesh data, metadata
	IModelManager* modelManager{ nullptr };
	// Client-server relations, packets...
	INetwork* network{ nullptr };
	// Dynamics, joints, ragdolls...
	IPhysics* physics{ nullptr };
	// Management of plugin libraries
	IPluginSystem* pluginSystem{ nullptr };

	// Client-specific stuff
	// The serverside should never use these

	// Sound sources, music, filters, reverb
	IAudio* audio{ nullptr };
	// Keyboard & mouse input
	IInput* input{ nullptr };
	// Rendering of 3D models, 2D surfs, text etc.
	IRenderSystem* renderer{ nullptr };
};

class IPlugin
{
public:
	virtual bool Init( const EngineAPI& api ) = 0;
	virtual void Shutdown() = 0;

	// Overridden by individual plugins
	virtual const char* GetPluginName() const = 0;

	// Overridden by plugin types, e.g. IApplication
	virtual const char* GetInterfaceName() const = 0;

	static constexpr const char* Name = "IPlugin";
};

using PluginFactoryFunction = IPlugin*();

class PluginRegistry final
{
public:
	PluginRegistry( const int& engineVersion )
		: engineVersion( engineVersion )
	{
	}

	template<typename PluginType>
	PluginRegistry& Register()
	{
		factories.push_back( []()
		{ 
			IPlugin* plugin = new PluginType();
			return plugin;
		} );

		return *this;
	}

	const int& GetEngineVersion() const
	{
		return engineVersion;
	}

	const Vector<PluginFactoryFunction*>& GetFactories() const
	{
		return factories;
	}

private:
	int engineVersion{ 0 };
	Vector<PluginFactoryFunction*> factories;
};

// There will be an 'ADM_API PluginRegistry GetPluginRegistry()'
// in the game DLL. It exports the client and the server game, while importing stuff from the engine
constexpr const char* PluginInterfaceFunctionName = "GetPluginRegistry";
using PluginInterfaceFunction = PluginRegistry*();
