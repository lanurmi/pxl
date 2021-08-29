#include <pxl/graphics/spritefont.h>
#include <pxl/utils/image_packer.h>
#include <pxl/engine.h>

using namespace pxl;

class SpriteFontDataTTF
{
public:
	SpriteFontDataTTF(const String& file, int size) : _font(file), _size(size)
	{

	}
	bool pixels(u32 ch, Color* pixels)
	{
		auto scale = _font.scale(_size);
		auto glyph = _font.glyph(ch);
		auto character = _font.character(glyph, scale);
		if(character.has_glyph) {
			_font.image(character, pixels);
			return true;
		}
		else
		{
			return false;
		}
	}
	Vec2 pixelSize(u32 ch)
	{
		auto scale = _font.scale(_size);
		auto glyph = _font.glyph(ch);
		auto character = _font.character(glyph, scale);
		return Vec2(character.width, character.height);
	}
	int kerning(u32 ch0, u32 ch1) const
	{
		auto scale = _font.scale(_size);
		auto glyph0 = _font.glyph(ch0);
		auto glyph1 = _font.glyph(ch1);
		return _font.kerning(glyph0, glyph1, scale);
	}
	float advance(u32 ch) const
	{
		auto scale = _font.scale(_size);
		auto glyph = _font.glyph(ch);
		auto character = _font.character(glyph, scale);
		return character.advance;
	}
	Vec2 offset(u32 ch) const
	{
		auto scale = _font.scale(_size);
		auto glyph = _font.glyph(ch);
		auto character = _font.character(glyph, scale);
		return Vec2(character.offset_x, character.offset_y);
	}
	int lineHeight() const
	{
		auto scale = _font.scale(_size);
		return _font.lineHeight() * scale;
	}
private:
	Font _font;
	int _size;
};

SpriteFont::SpriteFont(const String& file, int size, const CharacterRange& range)
{
	build(file, size, range);
}

SpriteFont::SpriteFont(SpriteFont&& src) noexcept
{
	_size = src._size;
	_lineHeight = src._lineHeight;
	_kernings = std::move(src._kernings);
	_characters = std::move(src._characters);
}

SpriteFont& SpriteFont::operator=(SpriteFont&& src) noexcept
{
	_size = src._size;
	_lineHeight = src._lineHeight;
	_characters = std::move(src._characters);
	_kernings = std::move(src._kernings);
	return *this;
}

void SpriteFont::build(const String& file, int size, const CharacterRange &range)
{
	_size = size;
	Color* buffer = new Color[size * size * 4];
	SpriteFontDataTTF font(file, size);
	_lineHeight = font.lineHeight();

	ImagePacker packer(1024, 1024);

	for (int i = range.from; i <= range.to; i++)
	{

		if (font.pixels(i, buffer))
		{
			auto size = font.pixelSize(i);
			packer.add(i, size.x, size.y, buffer);
		}
		Character ch;
		ch.advance = font.advance(i);
		ch.offset = font.offset(i);
		_characters.emplace(i, ch);
		
	}
	delete [] buffer;

	auto packed = packer.pack();
	for (auto& it : packed)
	{
		auto texture = Texture::create(it.image);
		for(auto &itt : it.entries)
		{

			_characters[itt.id].subtexture = Subtexture(texture, pxl::Rect(itt.x, itt.y, itt.width, itt.height));
		}
	}

	// Kernings

	for (u32 i = range.from; i <= range.to; i++)
	{
		for (u32 j = range.from; j <= range.to; j++)
		{
			auto kerning = font.kerning(i, j);
			if(kerning != 0)
			{
				u64 idx = ((i64)i << 32) | j;
				_kernings[idx] = kerning;
			}
		}
	}
}

const SpriteFont::Character* SpriteFont::character(int ch) const
{
	auto it = _characters.find(ch);
	if (it != _characters.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}

int SpriteFont::kerning(u32 ch0, u32 ch1) const
{
	u64 key = ((u64)ch0 << 32) | ch1;
	auto it = _kernings.find(key);
	if (it != _kernings.end())
	{
		return it->second;
	} 	
	else
	{
		return 0;
	}
}

int SpriteFont::lineHeight() const
{
	return _lineHeight;
}