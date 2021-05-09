#include <pxl/utils/content.h>
#include <pxl/image/image.h>
#include <pxl/engine.h>
#include <assert.h>

pxl::Content::Content(const string &contentFolder) : _content_folder(contentFolder)
{
}

pxl::TextureRef pxl::Content::LoadTexture(const string& file)
{

	auto pathToContent = pxl::path::combine(pxl::platform().applicationPath(), _content_folder);
	auto pathToFile = pxl::path::combine(pathToContent, file);

	auto it = _loaded_textures.find(file);
	if (it != _loaded_textures.end())
	{
		return it->second;
	}
	else
	{
		auto texture = pxl::Texture::create(pxl::Image(pathToFile));
		if (texture == nullptr)
		{
			assert(0);//err
		}
		else
		{
			_loaded_textures[file] = texture;
			return texture;
		}
	}
}