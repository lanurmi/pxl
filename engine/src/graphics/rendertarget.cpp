#include <pxl/graphics/rendertarget.h>
#include <pxl/engine.h>


pxl::RenderTargetRef pxl::RenderTarget::create(int width, int height)
{
	return pxl::graphics::createRenderTarget(width, height);
}

int pxl::RenderTarget::width() const
{
	return texture()->width();
}

int pxl::RenderTarget::height() const
{
	return texture()->height();
}
