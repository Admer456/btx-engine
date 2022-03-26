
#pragma once

// Shared systems
class ICore;            // sysinfo etc.   Server and Client
class IAnimation;       // animation,     Server and Client
class ICollision;       // collision,     Server and Client
class IConsole;         // cvars etc.     Server and Client
class IFileSystem;      // files, dirs    Server and Client
class IMaterialManager; // materials,     Server and Client
class IModelManager;    // models,        Server and Client
class IPhysics;         // physics,       Server and Client
// Clientside-only systems
class IAudio;           // sound,         Client
class IInput;           // input,         Client
class IRenderSystem;    // rendering,     Client

class IGame
{
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void RunFrame( const float deltaTime ) = 0;
    virtual void EmitGameState() = 0;
};

class IClient
{
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void RunFrame( const float deltaTime ) = 0;
    virtual void RenderFrame() = 0;
};

// The engine imports this stuff
struct gameLibraryExports
{
    IGame* game;
    IClient* client; // nullptr in dedicated server instances, server uses a dummy client implementation
};

// The game imports this stuff from the engine
struct gameLibraryImports
{
    // Shared stuff
    // Both the server & client game use these
    ICore*              core{ nullptr };            // system info, timing...
    IAnimation*         animation{ nullptr };       // animation blending, playback...
    ICollision*         collision{ nullptr };       // raycasts, object-to-object tests...
    IConsole*           console{ nullptr };         // printing, console vars, console commands...
    IFileSystem*        fileSystem{ nullptr };      // files, directories
    IMaterialManager*   materialManager{ nullptr }; // textures, surface properties
    IModelManager*      modelManager{ nullptr };    // model mesh data, metadata
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
