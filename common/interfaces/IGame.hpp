
#pragma once

// Shared systems
class ICore;            // sysinfo etc.   Server and Client
class IAnimation;       // animation,     Server and Client
class ICollision;       // collision,     Server and Client
class IConsole;         // cvars etc.     Server and Client
class IFileSystem;      // files, dirs    Server and Client
class IMaterialManager; // materials,     Server and Client
class IModelManager;    // models,        Server and Client
class INetwork;         // networking,    Server and Client
class IPhysics;         // physics,       Server and Client
// Clientside-only systems
class IAudio;           // sound,         Client
class IInput;           // input,         Client
class IRenderSystem;    // rendering,     Client

// Generic game interface. This is as simple as it gets, really
class IGame
{
public:
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;

	// The game may freely "delay" this
	// E.g. the server game updates at 20 Hz, 
	// while the client game updates every frame
	virtual void Update() = 0;
};

// The engine imports this stuff so it can update them
struct gameLibraryExports
{
	// nullptr potentially if you're implementing an SP-only game with no MP considerations
	IGame* server{ nullptr };
	// nullptr in dedicated server instances -> no renderer, no audio, no clientside FX
	IGame* client{ nullptr };
};

// The game imports this stuff from the engine
// The engine does not provide any type of save/load system, AI
// system, level/scene system, none of that, it is completely up to
// you to do it in whatever way you want
// 
// In fact, you are free to override any of these systems here in your
// game, like the physics or networking system, if for whatever reason
// you cannot modify the engine itself
struct gameLibraryImports
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

	// Client-specific stuff
	// The serverside should never use these

	// Sound sources, music, filters, reverb
	IAudio* audio{ nullptr };
	// Keyboard & mouse input
	IInput* input{ nullptr };
	// Rendering of 3D models, 2D surfs, text etc.
	IRenderSystem* renderer{ nullptr };
};

// There will be an 'extern "C" gameLibraryExports* ExchangeGameInterface( gameLibraryImports* )
// in the game DLL. It exports the client and the server game, while importing stuff from the engine
constexpr const char* GameInterfaceFunctionName = "ExchangeGameInterface";
using GameInterfaceFunction = gameLibraryExports * (gameLibraryImports*);
