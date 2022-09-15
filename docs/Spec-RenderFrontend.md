
# BTX render frontend draft

The game has a state which needs to be rendered: things like meshes, GUIs and particles. The engine has low-level tools for rendering (NVRHI which lets us use DX12 and Vulkan), that is means of issuing drawcalls, creating buffers on the GPU and so on. 

The render frontend is in essence the middleman who interprets this state and interacts with the engine's render backend. As such, it needs to have a simple and straightforward, high-level interface for the game API.

The idea is that the game programmer can write their own rendering pipeline. In theory, and hopefully in practice, you'll be able to have a "90s" render frontend, which renders lightmapped, diffuse-only surfaces, and a "Modern" render frontend, which renders everything dynamically and whatnot.

## Scope

We must delimit what is the concern of the frontend, backend and game separately.

**The game** *can* implement particle systems, animation, vegetation & grass.  
**The render frontend** *can* implement LODs, lighting, occlusion culling, batching, communicating with the render backend to issue drawcalls & does things with materials.  
**The render backend** implements the render hardware interface (in this case we're gonna be using NVRHI), does the actual rendering.

## API

The API is inspired by modern graphics APIs, in the sense that objects are created from descriptions.

```cpp
auto renderEntityDesc = RenderEntityDesc()
	.SetModel( ModelManager->Load( "models/tracksuit.btm" ) )
	.SetTransform( Mat4::Identity );

IRenderEntity* renderEntity = Renderer->CreateRenderEntity( renderEntityDesc );
```

Here you can already see a glimpse of what it could look like.

The game will be able to interact with the render frontend, and even directly with the render backend, but only through hooks (scroll down to *Render views*).

## Concepts

Let's move on to the concepts, or types of objects, that the render frontend interface deals with.

*Italic* fields are optional.  
"Custom parameters" means a 4D vector and a dictionary to store additional parameters that render frontends might make use of.  
"Render mask" is a simple 32-bit mask that conditionally excludes objects from rendering.

* **Render entities**
	* Transform
	* Model
	* *Bone buffer*
	* *Render mask*
	* *Custom parameters*
* **Render batch**
	* List of transforms
	* Render entity
	* *Render mask*
* **Render lights**
	* Transform
	* Type (point, spot, omni)
	* Colour
	* Range
	* *Material (spotlight cookie)*
	* *Render mask*
	* *Custom parameters*
* **Render volumes**
	* Transform
	* Bounding box
	* Material
	* *Model*
	* *Render mask*
	* *Custom parameters*
* **Render views**
	* Transform 
	* Vertical FOV
	* Near & far plane
	* Viewport size
	* Type (2D or 3D)
	* *Hook (for custom rendering)*
		* Function pointer
		* Moment (before rendering, after rendering)
	* *Material (for post-processing)*
	* *Render mask*
	* *Target (for render targets)*

If anything obvious is missing, it will be added in later revisions.
ImGui can be implemented in the game as a render frontend hook.
