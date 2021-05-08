#include <pxl/graphics/render_target.h>
#include <pxl/engine.h>


pxl::RenderTargetRef pxl::RenderTarget::create(int width, int height)
{
	return pxl::graphics().createRenderTarget(width, height);
}

int pxl::RenderTarget::width() const
{
	return getTexture()->width();
}

int pxl::RenderTarget::height() const
{
	return getTexture()->height();
}
