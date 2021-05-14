#pragma once

#include <pxl/assets/BMFont.h>
#include <pxl/math/rect.h>
#include <pxl/graphics/Subtexture.h>
#include <pxl/types.h>

namespace pxl
{
	class SpriteFont;
	using SpriteFontRef = std::shared_ptr<SpriteFont>;

	class SpriteFont
	{
	public:
		struct Glyph
		{
			Glyph();
			Subtexture subtexture;
			u16 x_offset;
			u16 y_offset;
			u16 x_advance;
			u32 kerning(u32 previousGlyph) const;
		private:
			friend class SpriteFont;
			map<u32, u32> _kerningAmount;
		};

		static SpriteFontRef create(const BMFont &font);
		const Glyph& character(u32 glyph) const;
		u16 lineHeight() const;
	private:
		SpriteFont(const BMFont& font);
		void build(const BMFont& font);
		map<u32, Glyph> _glyphs;
		u16 _line_height;
		
	};
}