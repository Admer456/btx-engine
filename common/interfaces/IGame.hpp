#pragma once

class ICommon;          // printing etc., Server and Client
class IAnimationSystem; // animation,     Server and Client
class ICollisionSystem; // collision,     Server and Client
class IMaterialManager; // materials,     Server and Client
class IModelManager;    // models,        Server and Client
class IPhysicsSystem;   // physics,       Server and Client
class IAudioSystem;     // sound,         Client
class IInputSystem;     // input,         Client
class IRenderSystem;    // rendering,     Client

class IGame
{
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void RunFrame( const float deltaTime ) = 0;
};

class IClient
{
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void RunFrame( const float deltaTime ) = 0;
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
    ICommon*            common{ nullptr };          // printing, system info
    IAnimationSystem*   animation{ nullptr };       // animation blending, playback...
    ICollisionSystem*   collision{ nullptr };       // raycasts, object-to-object tests...
    IMaterialManager*   materialManager{ nullptr }; // textures, surface properties
    IModelManager*      modelManager{ nullptr };    // model mesh data, metadata
    IPhysicsSystem*     physics{ nullptr };         // dynamics, joints, ragdolls...

    // Client-specific stuff
    // The serverside should never use these
    IAudioSystem*       audio{ nullptr };           // sound sources, music, filters, reverb
    IInputSystem*       input{ nullptr };           // keyboard & mouse input
    IRenderSystem*      renderer{ nullptr };        // rendering of 3D models, 2D surfs, text etc.
};

// There will be an 'extern "C" gameLibraryExports* ExchangeGameInterface( gameLibraryImports* )
// in the game DLL. It exports the client and the server game, while importing stuff from the engine
constexpr const char* GameInterfaceFunctionName = "ExchangeGameInterface";
using GameInterfaceFunction = gameLibraryExports* (gameLibraryImports*);
