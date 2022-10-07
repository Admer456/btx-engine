
#pragma once

namespace Render
{
	class IModel;

	struct EntityDesc final
	{
		adm::Mat4	transform{};
		IModel*		model{ nullptr };

		// bones must point to valid memory, otherwise null
		// if you won't be using any animations
		uint8_t		numBones{ 0 };
		adm::Mat4*	bones{ nullptr };

		uint32_t	renderMask{ RenderMaskDefault };
		adm::Vec4	shaderParameters[2];
	};

	class IEntity
	{
	public:
		virtual EntityDesc& GetDesc() = 0;
		virtual const EntityDesc& GetDesc() const = 0;
	};
}
