
#pragma once

namespace Render
{
	struct BatchDesc final
	{

	};

	class IBatch
	{
	public:
		virtual BatchDesc& GetDesc() = 0;
		virtual const BatchDesc& GetDesc() const = 0;
	};
}
