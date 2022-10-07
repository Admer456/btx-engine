
#pragma once

// Todo: adapt this: https://github.com/Admer456/btx-engine/blob/master/docs/Spec-RenderFrontend.md#concepts

namespace Render
{
	// These are defined in rendering/IRender*.hpp
	struct BatchDesc;
	struct EntityDesc;
	struct LightDesc;
	struct ViewDesc;
	struct VolumeDesc;
	// These are implemented by the render backend
	class IBatch;
	class IEntity;
	class ILight;
	class IView;
	class IVolume;
}

class IRenderFrontend : public IPlugin
{
public:
	DeclarePluginInterface( IRenderFrontend );

	virtual Render::IBackend*			GetBackend() const = 0;

	virtual void						RenderView( const Render::IView* view ) = 0;

	virtual void						DebugLine( adm::Vec3 start, adm::Vec3 end, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true) = 0;
	virtual void						DebugRay( adm::Vec3 start, adm::Vec3 direction, float length = 1.0f, bool withArrowhead = false, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true ) = 0;
	virtual void						DebugBox( adm::Vec3 min, adm::Vec3 max, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true ) = 0;
	virtual void						DebugCube( adm::Vec3 position, float extents = 1.0f, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true ) = 0;
	virtual void						DebugSphere( adm::Vec3 position, float extents = 1.0f, adm::Vec3 colour = adm::Vec3( 1.0f ), float life = 0.0f, bool depthTest = true ) = 0;

	virtual Render::IBatch* 			CreateBatch( const Render::BatchDesc& desc ) = 0;
	virtual bool 						DestroyBatch( Render::IBatch* batch ) = 0;
	virtual const Vector<Render::IBatch*>& GetBatches() const = 0;

	virtual Render::IEntity* 			CreateEntity( const Render::EntityDesc& desc ) = 0;
	virtual bool 						DestroyEntity( Render::IEntity* entity ) = 0;
	virtual const Vector<Render::IEntity*>& GetEntities() const = 0;

	virtual Render::ILight* 			CreateLight( const Render::LightDesc& desc ) = 0;
	virtual bool 						DestroyLight( Render::ILight* light ) = 0;
	virtual const Vector<Render::ILight*>& GetLights() const = 0;

	virtual Render::IView* 				CreateView( const Render::ViewDesc& desc ) = 0;
	virtual bool 						DestroyView( Render::IView* view ) = 0;
	virtual const Vector<Render::IView*>& GetViews() const = 0;

	virtual Render::IVolume* 			CreateVolume( const Render::VolumeDesc& desc ) = 0;
	virtual bool 						DestroyVolume( Render::IVolume* volume ) = 0;
	virtual const Vector<Render::IVolume*>& GetVolumes() const = 0;
};
