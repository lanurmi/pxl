#include <pxl/backbuffer.h>
#include <pxl/backends/graphics_backend.h>
#include <pxl/backends/platform_backend.h>

namespace
{
	class BackBufferTarget : public pxl::RenderTarget
	{
	public:
		BackBufferTarget() : pxl::RenderTarget()
		{
		}
		int width() const override
		{
			return pxl::platform::drawSize().x;
		}
		int height() const override 
		{
			return pxl::platform::drawSize().y;
		}
		void clear(const pxl::Color& color) override
		{
			pxl::graphics::clearBackbuffer(color);
		}
		pxl::TextureRef texture() const override
		{
			return nullptr;
		}
	};
}

namespace pxl
{
	const RenderTargetRef backbuffer = RenderTargetRef(new BackBufferTarget());
}

