#pragma once
#include <pxl/types.h>

namespace pxl
{
	struct BMFont
	{
		struct Info
		{
			string face;
			i16 size;
			i16 bold;
			i16 italic;
			string charset;
			i16 unicode;
			i16 strechH;
			i16 smooth;
			i16 aa;
			i16 padding_up;
			i16 padding_right;
			i16 padding_down;
			i16 padding_left;
			i16 spacing_horizontal;
			i16 spacing_vertical;
			i16 outline;
		};
		struct Common
		{
			i16 line_height;
			i16 base;
			i16 scale_w;
			i16 scale_h;
			i16 pages;
			i16 packed;
			i16 alpha_channel;
			i16 red_channel;
			i16 green_channel;
			i16 blue_channel;
		};
		struct Page
		{
			i16 id;
			string file;
		};
		struct Char
		{
			u32 id;
			i16 x;
			i16 y;
			i16 width;
			i16 height;
			i16 x_offset;
			i16 y_offset;
			i16 x_advance;
			i16 page;
			i16 channel;
		};

		struct Kerning
		{
			u32 first;
			u32 second;
			i16 amount;
		};

		BMFont(const string& file);
		Info info;
		Common commmon;
		vector<Page> pages;
		vector<Char> chars;
		vector<Kerning> kernings;
	private:
		void load(const string& file);
		void forEveryKeyValuePair(const string& line, std::function<void(const string& key, const string& value)> func);
	};
}