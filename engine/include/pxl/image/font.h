#pragma once
#include <pxl/types.h>
#include <pxl/graphics/texture.h>
#include <functional>

namespace pxl
{
	struct FontCharacter
	{
		u32 glyph;
		u16 x;
		u16 y;
		u16 width;
		u16 height;
		u16 x_offset;
		u16 y_offset;
		u16 x_advance;
	};

	struct FontKerning
	{
		u32 first;
		u32 second;
		u32 amount;
	};

	class Font
	{
	public:
		Font(const string& file);
		const map<u32, FontCharacter>& characters() const;
		const vector<FontKerning>& kernings() const;
		const string& name() const;
		const Image& image() const;
		u16 lineHeight() const;
	private:
		Image _image;
		string _name;
		map<u32, FontCharacter> _characters;
		vector<FontKerning> _kerning;
		u16 _line_height;
	};
}