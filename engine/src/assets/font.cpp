#include <pxl/assets/font.h>
#include <pxl/utils/filestream.h>
#include <pxl/log.h>

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION

#include <stb/stb_truetype.h>

using namespace pxl;

CharacterRange::CharacterRange(u32 from, u32 to) : from(from), to(to) {}

CharacterRange CharacterRange::ASCII = CharacterRange(32, 128);


pxl::Font::Font(const pxl::String &path)
{
	load(path);
}

Font::Font(Font&& src) noexcept
{
	_data = src._data;
	_ascent = src._ascent;
	_descent = src._descent;
	_line_gap = src._line_gap;
	src._data = nullptr;
}

pxl::Font::~Font()
{
	delete (stbtt_fontinfo*)_handle;
	delete[] _data;
}

void Font::load(const String& path)
{
	FileStream stream(path, file::FileMode::ReadBinary);
	auto length = stream.length();
	_data = new u8[length];
	stream.read(_data, length);

	_handle = nullptr;
	_handle = new stbtt_fontinfo();
	auto f = (stbtt_fontinfo*)_handle;

	stbtt_InitFont(f, _data, 0);

	stbtt_GetFontVMetrics(f, &_ascent, &_descent, &_line_gap);
}

int Font::ascent() const
{
	return _ascent;
}

int Font::descent() const
{
	return _descent;
}

int Font::lineGap() const
{
	return _line_gap;
}

int Font::height() const
{
	return ascent() - descent();
}

int Font::lineHeight() const
{
	return height() - lineGap();
}

float Font::scale(float size) const
{
	auto f = (stbtt_fontinfo*)_handle;
	return stbtt_ScaleForMappingEmToPixels(f, size);
}

float Font::kerning(int glyph0, int glyph1, float scale) const
{
	auto f = (stbtt_fontinfo*)_handle;
	return stbtt_GetGlyphKernAdvance(f, glyph0, glyph1) * scale;
}

bool Font::image(const Font::Character& ch, Color* pixels) const
{
	auto f = (stbtt_fontinfo*)_handle;
	if (ch.has_glyph)
	{
		unsigned char *ptr = (unsigned char*)pixels;
		stbtt_MakeGlyphBitmap(f, ptr, ch.width, ch.height, ch.width, ch.scale, ch.scale, ch.glyph);
		auto l = ch.width * ch.height;
		for (int a = (l - 1) * 4, b = (l - 1); b >= 0; a -= 4, b -= 1)
		{
			ptr[a + 0] = ptr[b];
			ptr[a + 1] = ptr[b];
			ptr[a + 2] = ptr[b];
			ptr[a + 3] = ptr[b];
		}
		return true;
	}
	else
	{
		return true;
	}
}

int Font::glyph(int codepoint) const
{
	auto f = (stbtt_fontinfo*)_handle;
	return stbtt_FindGlyphIndex(f, codepoint);
}

Font::Character Font::character(int glyph, float scale) const
{
	int advance, offsetx, x0,y0,x1,y1;
	auto f = (stbtt_fontinfo*)_handle;
	stbtt_GetGlyphHMetrics(f, glyph, &advance, &offsetx);
	stbtt_GetGlyphBitmapBox(f, glyph, scale, scale, &x0, &y0, &x1, &y1);

	Character ch;
	ch.glyph = glyph;
	ch.width = x1 - x0;
	ch.height = y1 - y0;
	ch.advance = advance * scale;
	ch.offset_x = offsetx * scale;
	ch.offset_y = static_cast<float>(y0);
	ch.scale = scale;
	ch.has_glyph = (ch.width > 0 && ch.height > 0 && stbtt_IsGlyphEmpty(f, glyph) == 0);
	return ch;
}