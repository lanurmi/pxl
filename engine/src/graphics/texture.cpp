#include <pxl/graphics/texture.h>
#include <pxl/engine.h>
#include <assert.h>

pxl::TextureRef pxl::Texture::create(int width, int height, pxl::TextureFormat format, pxl::u8* data)
{
	assert(width > 0 && height > 0);
	auto tex = pxl::graphics::createTexture(width, height, format);
	if (tex != nullptr && data != nullptr)
	{
		tex->setData(data);
	}

	return tex;
}

pxl::TextureRef pxl::Texture::create(int width, int height, pxl::TextureFormat format)
{
	assert(width > 0 && height > 0);
	auto tex = pxl::graphics::createTexture(width, height, format);
	return tex;
}


pxl::TextureRef pxl::Texture::create(const pxl::Image& image)
{
	auto data = image.pixels();
	auto w = image.width();
	auto h = image.height();
	return pxl::Texture::create(w, h, pxl::TextureFormat::RGBA, &data->r);
}

pxl::Texture::Texture()
{
}