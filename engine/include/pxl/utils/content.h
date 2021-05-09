#pragma once
#include <pxl/graphics/texture.h>
#include <pxl/types.h>

namespace pxl
{
	class Content
	{
	public:
		Content(const string &contentFolder);
		TextureRef LoadTexture(const string& file);
	private:
		string _content_folder;
		map<string, TextureRef> _loaded_textures;
	};
}
