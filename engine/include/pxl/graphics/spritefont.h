#pragma once

#include <pxl/assets/Font.h>
#include <pxl/math/rect.h>
#include <pxl/graphics/Subtexture.h>
#include <pxl/containers/map.h>
#include <pxl/types.h>
#include <pxl/math/vec2.h>

namespace pxl
{
	class SpriteFont;

	class SpriteFont
	{
	public:
		struct Character
		{
			Subtexture subtexture;
			float advance;
			Vec2 offset;
		};
		SpriteFont() {}
		SpriteFont(const String&file, int size, const CharacterRange& range);
		SpriteFont& operator=(const SpriteFont&) = delete;
		SpriteFont(SpriteFont&& src) noexcept;
		SpriteFont& operator=(SpriteFont&& src) noexcept;
		const Character *character(int ch) const;
		int kerning(u32 ch0, u32 ch1) const;
		int lineHeight() const;
	private:
		void build(const String&file, int size, const CharacterRange& range);
		int _lineHeight;
		int _size;
		Map<u32, Character> _characters;
		Map<u64, int> _kernings;
	};
}