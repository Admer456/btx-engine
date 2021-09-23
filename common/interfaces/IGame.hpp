#pragma once

class ICommon;          // printing etc., for Game and Client
class IAnimationSystem; // animation,     for Game and Client
class IAudioSystem;     // sound,         for Client
class ICollisionSystem; // collision,     for Game and Client
class IInputSystem;     // input,         for Client
class IMaterialManager; // materials,     for Game and Client
class IModelManager;    // models,        for Game and Client
class IPhysicsSystem;   // physics,       for Game and Client
class IRenderSystem;    // rendering,     for Client

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
    IClient* client;
};

// The game imports this stuff from the engine
struct gameLibraryImports
{
    // Shared stuff
    ICommon*            common{ nullptr };
    IAnimationSystem*   animation{ nullptr };
    ICollisionSystem*   collision{ nullptr };
    IMaterialManager*   materialManager{ nullptr };
    IModelManager*      modelManager{ nullptr };
    IPhysicsSystem*     physics{ nullptr };

    // Client-specific stuff
    IAudioSystem*       audio{ nullptr };
    IInputSystem*       input{ nullptr };
    IRenderSystem*      renderer{ nullptr };
};

// There will be an 'extern "C" gameLibraryExports* ExchangeGameInterface( gameLibraryImports* )
// in the game DLL. It exports the client and the server game, while importing stuff from the engine
constexpr const char* GameInterfaceFunctionName = "ExchangeGameInterface";
using GameInterfaceFunction = gameLibraryExports* (gameLibraryImports*);
