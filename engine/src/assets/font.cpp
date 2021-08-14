#include <pxl/assets/font.h>
#include <pxl/utils/filestream.h>

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include <pxl/3rdparty/stb_truetype.h>

using namespace pxl;

CharacterRange::CharacterRange(u32 from, u32 to) : from(from), to(to) {}

CharacterRange CharacterRange::ASCII = CharacterRange(32, 128);

static String getFontName(stbtt_fontinfo* font, int nameId)
{
	int length = 0;
	const char* ptr = stbtt_GetFontNameString(font, &length,
		STBTT_PLATFORM_ID_MICROSOFT,
		STBTT_MS_EID_UNICODE_BMP,
		STBTT_MS_LANG_ENGLISH,
		nameId);

	return String(ptr, length);
}

pxl::Font::Font(const pxl::String &path)
{
	load(path);
}

Font::Font(Font&& src) noexcept
{
	_font = src._font;
	_data = src._data;
	_family_name = src._family_name;
	_style_name = src._style_name;
	_ascent = src._ascent;
	_descent = src._descent;
	_line_gap = src._line_gap;

	src._family_name = "";
	src._style_name = "";
	src._font = nullptr;
	src._data = nullptr;
}

pxl::Font::~Font()
{
	delete [] _data;
	delete _font;
}

void Font::load(const String& path)
{
	FileStream stream(path, file::FileMode::ReadBinary);
	auto length = stream.length();
	_data = new u8[length];
	stream.read(_data, length);

	_font = new stbtt_fontinfo();
	stbtt_InitFont(_font, _data, 0);

	_family_name = getFontName(_font, 1);
	_style_name = getFontName(_font, 2);

	stbtt_GetFontVMetrics(_font, &_ascent, &_descent, &_line_gap);
}

String Font::familyName() const
{
	return _family_name;
}

String Font::styleName() const
{
	return _style_name;
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
	return stbtt_ScaleForMappingEmToPixels(_font, size);
}

float Font::kerning(int glyph0, int glyph1, float scale) const
{
	return stbtt_GetGlyphKernAdvance(_font, glyph0, glyph1) * scale;
}

bool Font::image(const Font::Character& ch, Color* pixels) const
{
	if (ch.has_glyph)
	{
		unsigned char *ptr = (unsigned char*)pixels;
		stbtt_MakeGlyphBitmap(_font, ptr, ch.width, ch.height, ch.width, ch.scale, ch.scale, ch.glyph);
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
	return stbtt_FindGlyphIndex(_font, codepoint);
}

Font::Character Font::character(int glyph, float scale) const
{
	int advance, offsetx, x0,y0,x1,y1;

	stbtt_GetGlyphHMetrics(_font, glyph, &advance, &offsetx);
	stbtt_GetGlyphBitmapBox(_font, glyph, scale, scale, &x0, &y0, &x1, &y1);


	Character ch;
	ch.glyph = glyph;
	ch.width = x1 - x0;
	ch.height = y1 - y0;
	ch.advance = advance * scale;
	ch.offset_x = offsetx * scale;
	ch.offset_y = static_cast<float>(y0);
	ch.scale = scale;
	ch.has_glyph = (ch.width > 0 && ch.height > 0 && stbtt_IsGlyphEmpty(_font, glyph) == 0);
	return ch;
}