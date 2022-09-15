
# Layers

The structure of the entire engine is admittedly pretty weird.

There's 3 layers generally:
* adm-utils -> specialised supplement to the C++ standard library, to implement things like `Vec3`, `Octree`, `Timer` and so on
* btx-engine (this repository) -> a framework that implements a console system, virtual filesystem, input system, plugin system and generally acts as a wrapper around other libraries
* btx-sdk (does not exist yet) -> a full game SDK implemented on top of the engine, having physics, networking, a vegetation system, a particle system, map loading, main menus, AI and other truly basic necessities for creating a retro FPS game

And on top of that, there are plugins for:
* the audio system
* the renderer
* model formats
* texture formats

# Inner engine structure

*TODO: write*
