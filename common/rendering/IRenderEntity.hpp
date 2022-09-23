
#pragma once

class IModel;

struct RenderEntityDesc final
{
	adm::Mat4	transform{};
	IModel*		model{ nullptr };
	
	uint8_t		numBones{ 0 };
	adm::Mat4*	bones{ nullptr };

	uint32_t	renderMask{ RenderMaskDefault };
	adm::Vec4	shaderParameters[2];
};

class IRenderEntity
{
public:

};
