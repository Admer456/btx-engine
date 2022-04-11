
#pragma once

#include "common/Precompiled.hpp"

inline gameLibraryImports* Engine = nullptr;

// Copy pasted from gameLibraryImports as a convenience layer, less typing
// 
// Shared stuff
// Both the server & client game use these
inline ICore* Core = nullptr;                       // system info, timing...
inline IAnimation* Animation = nullptr;             // animation blending, playback...
inline ICollision* Collision = nullptr;             // raycasts, object-to-object tests...
inline IConsole* Console = nullptr;                 // printing, console vars, console commands...
inline IFileSystem* FileSystem = nullptr;           // files, directories
inline IMaterialManager* MaterialManager = nullptr; // textures, surface properties
inline IModelManager* ModelManager = nullptr;       // model mesh data, metadata
inline INetwork* Network = nullptr;                 // client-server relations, packets...
inline IPhysics* Physics = nullptr;                 // dynamics, joints, ragdolls...

// Client-specific stuff
// The serverside should never use these
inline IAudio* Audio = nullptr;           // sound sources, music, filters, reverb
inline IInput* Input = nullptr;           // keyboard & mouse input
inline IRenderSystem* Renderer = nullptr; // rendering of 3D models, 2D surfs, text etc.

namespace detail
{
	extern CVarList GameCVarList;
}

using CVar = CVarTemplate<detail::GameCVarList, Console>;
