
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
    ICore*              core{ nullptr };            // system info, timing...
    IAnimation*         animation{ nullptr };       // animation blending, playback...
    ICollision*         collision{ nullptr };       // raycasts, object-to-object tests...
    IConsole*           console{ nullptr };         // printing, console vars, console commands...
    IFileSystem*        fileSystem{ nullptr };      // files, directories
    IMaterialManager*   materialManager{ nullptr }; // textures, surface properties
    IModelManager*      modelManager{ nullptr };    // model mesh data, metadata
    INetwork*           network{ nullptr };         // client-server relations, packets...
    IPhysics*           physics{ nullptr };         // dynamics, joints, ragdolls...

    // Client-specific stuff
    // The serverside should never use these
    IAudio*             audio{ nullptr };           // sound sources, music, filters, reverb
    IInput*             input{ nullptr };           // keyboard & mouse input
    IRenderSystem*      renderer{ nullptr };        // rendering of 3D models, 2D surfs, text etc.
};

// There will be an 'extern "C" gameLibraryExports* ExchangeGameInterface( gameLibraryImports* )
// in the game DLL. It exports the client and the server game, while importing stuff from the engine
constexpr const char* GameInterfaceFunctionName = "ExchangeGameInterface";
using GameInterfaceFunction = gameLibraryExports* (gameLibraryImports*);
