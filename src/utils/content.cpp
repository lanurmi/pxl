#include <pxl/utils/content.h>
#include <pxl/image/image.h>
#include <assert.h>

pxl::Content::Content()
{
}

pxl::TextureRef pxl::Content::LoadTexture(const string& file)
{
	auto it = _loaded_textures.find(file);
	if (it != _loaded_textures.end())
	{
		return it->second;
	}
	else
	{
		auto texture = pxl::Texture::create(pxl::Image(file));
		assert(texture);
		return texture;
	}
}