
#pragma once

namespace Render
{
	class IMaterial;
	class ITexture;

	struct HookMoment
	{
		enum Type
		{
			Before,
			After
		};
	};

	using HookFn = void( IBackend* renderBackend );

	struct ViewDesc final
	{
		adm::Mat4 viewMatrix{};
		adm::Mat4 projectionMatrix{};
		adm::Vec2 viewportSize{};

		// Render hooks allow you to render your own stuff on top, e.g. a GUI
		// hook is called when the current render view is being drawn, when exactly
		// depends on hookMoment
		HookMoment::Type moment{ HookMoment::After };
		std::function<HookFn> hook{ nullptr };
		// Post-process material to apply when rendering this view
		// Todo: maybe a separate subsystem for post-processing stacks?
		IMaterial* postProcessMaterial{ nullptr };
		// Texture to render the view into
		ITexture* renderTarget{ nullptr };
		// The render mask of this view
		// Objects will render if (view->renderMask & renderObject->renderMask)
		uint32_t renderMask{ RenderMaskDefault };
	};

	class IView
	{
	public:
		virtual ViewDesc& GetDesc() = 0;
		virtual const ViewDesc& GetDesc() const = 0;
	};
}
