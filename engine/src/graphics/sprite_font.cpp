#include <pxl/graphics/sprite_font.h>
#include <pxl/engine.h>

using namespace pxl;


SpriteFont::Glyph::Glyph()
{

}

u32 SpriteFont::Glyph::kerning(u32 glyph) const
{
	auto it = _kerningAmount.find(glyph);
	if (it == _kerningAmount.end()) return 0;
	return it->second;
}


SpriteFontRef SpriteFont::create(const BMFont &font)
{
	return SpriteFontRef(new SpriteFont(font) );
}



SpriteFont::SpriteFont(const BMFont& font)
{;
	build(font);
}

const SpriteFont::Glyph& SpriteFont::character(u32 glyph) const
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

void SpriteFont::build(const BMFont& font)
{
	auto texture = Texture::create(Image(font.pages[0].file));
	for (auto& it : font.chars)
	{
		SpriteFont::Glyph glyph;
		glyph.subtexture.set(texture, Rect(it.x, it.y, it.width, it.height));
		glyph.x_advance = it.x_advance;
		glyph.x_offset = it.x_offset;
		glyph.y_offset = it.y_offset;
		_glyphs[it.id] = glyph;
	}
	for (auto& it : font.kernings)
	{
		_glyphs[it.second]._kerningAmount[it.first] = it.amount;
	}
	_line_height = font.commmon.line_height;
}