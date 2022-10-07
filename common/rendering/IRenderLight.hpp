
#pragma once

namespace Render
{
	struct LightDesc final
	{

	};

	class ILight
	{
	public:
		virtual LightDesc& GetDesc() = 0;
		virtual const LightDesc& GetDesc() const = 0;
	};
}
