#pragma once
#include <pxl/graphics/texture.h>
#include <pxl/graphics/sprite_font.h>
#include <pxl/types.h>

namespace pxl
{
	class Content
	{
	public:
		Content(const string &contentFolder);
		TextureRef LoadTexture(const string& file);
		SpriteFontRef LoadFont(const string& file);
	private:
		string _content_folder;
		map<string, TextureRef> _loaded_textures;
		map<string, SpriteFontRef> _loaded_fonts;
	};
}
