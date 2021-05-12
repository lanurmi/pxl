#include <pxl/graphics/sprite_font.h>
#include <pxl/engine.h>

using namespace pxl;


SpriteFontGlyph::SpriteFontGlyph(const TextureRef& texture, const FontCharacter& chr, const vector<FontKerning>& kerningPairs) : subtexture(texture, pxl::Rect(chr.x, chr.y, chr.width, chr.height))
{
	x_offset = chr.x_offset;
	y_offset = chr.y_offset;
	x_advance = chr.x_advance;
	for (auto& it : kerningPairs)
	{
		if (it.second == chr.glyph)
		{
			if (it.amount != 0)
			{
				_kerningAmount[it.first] = it.amount;
			}
		}
	}

}


u32 SpriteFontGlyph::kerning(u32 glyph) const
{
	auto it = _kerningAmount.find(glyph);
	if (it == _kerningAmount.end()) return 0;
	return it->second;
}


SpriteFontRef SpriteFont::create(const Font &font)
{
	return SpriteFontRef(new SpriteFont(font) );
}



SpriteFont::SpriteFont(const Font& font)
{;
	build(font);
}

const SpriteFontGlyph& SpriteFont::character(u32 glyph) const
{
	auto it = _glyphs.find(glyph);
	if (it == _glyphs.end())
	{
		log().error("Glyph not found in spritefont");
		assert(0);
	}
	return it->second;
}

u16 SpriteFont::lineHeight() const
{
	return _line_height;
}

void SpriteFont::build(const Font& font)
{
	const auto& glyphs = font.characters();
	const auto& kernings = font.kernings();
	auto texture = Texture::create(font.image());
	for (auto& it : glyphs)
	{
		_glyphs.insert(std::make_pair(it.first, SpriteFontGlyph(texture, it.second, kernings)));
	}
	_line_height = font.lineHeight();
}
