#pragma once

#include <pxl/image/font.h>
#include <pxl/math/rect.h>
#include <pxl/graphics/Subtexture.h>
#include <pxl/types.h>

namespace pxl
{
	struct SpriteFontGlyph
	{
		SpriteFontGlyph(const TextureRef& texture, const FontCharacter& chr, const vector<FontKerning>& kerningPairs);
		Subtexture subtexture;
		u16 x_offset;
		u16 y_offset;
		u16 x_advance;
		u32 kerning(u32 previousGlyph) const;
	private:
		map<u32, u32> _kerningAmount;
		
	};

	class SpriteFont;
	using SpriteFontRef = std::shared_ptr<SpriteFont>;

	class SpriteFont
	{
	public:
		static SpriteFontRef create(const Font &font);
		const SpriteFontGlyph& character(u32 glyph) const;
		u16 lineHeight() const;
	private:
		SpriteFont(const Font& font);
		void build(const Font& font);
		map<u32, SpriteFontGlyph> _glyphs;
		u16 _line_height;
		
	};
}