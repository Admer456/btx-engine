
#pragma once

// Todo: adapt this: https://github.com/Admer456/btx-engine/blob/master/docs/Spec-RenderFrontend.md#concepts

// These are defined in rendering/IRender*.hpp
struct RenderBatchDesc;
struct RenderEntityDesc;
struct RenderLightDesc;
struct RenderViewDesc;
struct RenderVolumeDesc;

class IRenderBatch;
class IRenderEntity;
class IRenderLight;
class IRenderView;
class IRenderVolume;

class IRenderFrontend
{
public:
	virtual bool 						Init() = 0;
	virtual void 						Shutdown() = 0;

	virtual void						RenderView( const IRenderView* view ) = 0;

	virtual void						DebugLine( adm::Vec3 start, adm::Vec3 end, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true) = 0;
	virtual void						DebugRay( adm::Vec3 start, adm::Vec3 direction, float length = 1.0f, bool withArrowhead = false, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true ) = 0;
	virtual void						DebugBox( adm::Vec3 min, adm::Vec3 max, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true ) = 0;
	virtual void						DebugCube( adm::Vec3 position, float extents = 1.0f, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true ) = 0;
	virtual void						DebugSphere( adm::Vec3 position, float extents = 1.0f, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true ) = 0;

	virtual IRenderBatch* 				CreateBatch( const RenderBatchDesc& desc ) = 0;
	virtual bool 						DestroyBatch( IRenderBatch* batch ) = 0;
	virtual const Vector<IRenderBatch*>& GetBatches() const = 0;

	virtual IRenderEntity* 				CreateEntity( const RenderEntityDesc& desc ) = 0;
	virtual bool 						DestroyEntity( IRenderEntity* entity ) = 0;
	virtual const Vector<IRenderEntity*>& GetEntities() const = 0;

	virtual IRenderLight* 				CreateLight( const RenderLightDesc& desc ) = 0;
	virtual bool 						DestroyLight( IRenderLight* light ) = 0;
	virtual const Vector<IRenderLight*>& GetLights() const = 0;

	virtual IRenderView* 				CreateView( const RenderViewDesc& desc ) = 0;
	virtual bool 						DestroyView( IRenderView* view ) = 0;
	virtual const Vector<IRenderView*>& GetViews() const = 0;

	virtual IRenderVolume* 				CreateVolume( const RenderVolumeDesc& desc ) = 0;
	virtual bool 						DestroyVolume( IRenderVolume* volume ) = 0;
	virtual const Vector<IRenderVolume*>& GetVolumes() const = 0;
};
