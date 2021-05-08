#pragma once
#include <pxl/graphics/texture.h>
#include <pxl/types.h>

namespace pxl
{
	class Content
	{
	public:
		Content();
		TextureRef LoadTexture(const string& file);
	private:
		map<string, TextureRef> _loaded_textures;
	};
}
