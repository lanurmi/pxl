#pragma once

#include <pxl/types.h>
#include <pxl/math/color.h>
#include <pxl/containers/string.h>

namespace pxl
{
	struct CharacterRange
	{
	public:
		CharacterRange() = default;
		CharacterRange(u32 from, u32 to);
		u32 from;
		u32 to;
		static CharacterRange ASCII;
	};

	class Font
	{
	public:
		struct Character
		{
			int glyph = 0;
			int width = 0;
			int height = 0;
			float advance = 0.0f;
			float offset_x = 0.0f;
			float offset_y = 0.0f;
			float scale = 0.0f;
			bool has_glyph = false;
		};

		Font(const String &path);
		Font(Font &&src) noexcept;
		Font(const Font&) = delete;
		~Font();
		String familyName() const;
		String styleName() const;
		int ascent() const;
		int descent() const;
		int lineGap() const;
		int height() const;
		int lineHeight() const;
		float scale(float size) const;
		float kerning(int glyph0, int glyph1, float scale) const;
		Character character(int glyph, float scale) const;
		int glyph(int codepoint) const;
		bool image(const Font::Character &ch, Color *pixels) const;
	private:
		void load(const String &path);
		void* _data;
		String _family_name;
		String _style_name;
		int _ascent;
		int _descent;
		int _line_gap;
	};
}