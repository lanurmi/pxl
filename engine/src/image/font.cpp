#include <pxl/image/font.h>
#include <pxl/engine.h>
#include <pxl/filesystem.h>
#include <iostream>
#include <sstream>

using namespace pxl;


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

	BMFont(const string& file)
	{
		load(file);
	}

	Info info;
	Common commmon;
	vector<Page> pages;
	vector<Char> chars;
	vector<Kerning> kernings;

	private:
	void load(const string &file)
	{
		auto fileRef = file::File::create(file);
		while (!fileRef->eof())
		{
			string line = fileRef->line();
			std::stringstream lineStream;
			lineStream << line;
			string lineType;
			lineStream >> lineType;
			if (lineType == "info")
			{
				forEveryKeyValuePair(line, [this](const string& key, const string& value)
					{
						std::stringstream conv;
						conv << value;
						if (key == "face")
						{
							conv >> info.face;
						}
						else if (key == "size")
						{
							conv >> info.size;
						}
						else if (key == "bold")
						{
							conv >> info.bold;
						}
						else if (key == "italic")
						{
							conv >> info.italic;
						}
						else if (key == "charset")
						{
							conv >> info.charset;
						}
						else if (key == "unicode")
						{
							conv >> info.unicode;
						}
						else if (key == "stretchH")
						{
							conv >> info.strechH;
						}
						else if (key == "smooth")
						{
							conv >> info.smooth;
						}
						else if (key == "aa")
						{
							conv >> info.aa;
						}
						else if (key == "padding")
						{
							string str;
							conv >> str;
							int result = sscanf(str.c_str(), "%hi,%hi,%hi,%hi", &info.padding_up, &info.padding_right, &info.padding_down, &info.padding_left);
							if (result != 4)
							{
								log().error("error while reading padding from bmfont");
								assert(0);
							}
						}
						else if (key == "spacing")
						{
							string str;
							conv >> str;
							int result = sscanf(str.c_str(), "%hi,%hi", &info.spacing_horizontal, &info.spacing_vertical);
							if (result != 2)
							{
								log().error("error while reading spacing from bmfont");
								assert(0);
							}
						}
						else if (key == "outline")
						{
							info.face = value;
						}
					});
			}
			else if (lineType == "common")
			{
				forEveryKeyValuePair(line, [this](const string& key, const string& value)
					{
						std::stringstream conv;
						conv << value;
						if (key == "lineHeight")
						{
							conv >> commmon.line_height;
						}
						else if (key == "base")
						{
							conv >> commmon.base;
						}
						else if (key == "scale_w")
						{
							conv >> commmon.scale_w;
						}
						else if (key == "scale_h")
						{
							conv >> commmon.scale_h;
						}
						else if (key == "pages")
						{
							conv >> commmon.pages;
						}
						else if (key == "packged")
						{
							conv >> commmon.packed;
						}
						else if (key == "alphaChnl")
						{
							conv >> commmon.alpha_channel;
						}
						else if (key == "redChnl")
						{
							conv >> commmon.red_channel;
						}
						else if (key == "greenChnl")
						{
							conv >> commmon.green_channel;
						}
						else if (key == "blueChnl")
						{
							conv >> commmon.blue_channel;
						}
					});
			}
			else if (lineType == "page")
			{
				Page page;
				forEveryKeyValuePair(line, [this, &page](const string& key, const string& value)
					{
						std::stringstream conv;
						conv << value;
						if (key == "id")
						{
							conv >> page.id;
						}
						else if (key == "file")
						{
							conv >> page.file;
						}
					});
				pages.emplace_back(page);
			}
			else if (lineType == "char")
			{
				Char character;
				forEveryKeyValuePair(line, [this, &character](const string& key, const string& value)
					{
						std::stringstream conv;
						conv << value;
						if (key == "id")
						{
							conv >> character.id;
						}
						else if (key == "x")
						{
							conv >> character.x;
						}
						else if (key == "y")
						{
							conv >> character.y;
						}
						else if (key == "width")
						{
							conv >> character.width;
						}
						else if (key == "height")
						{
							conv >> character.height;
						}
						else if (key == "xoffset")
						{
							conv >> character.x_offset;
						}
						else if (key == "yoffset")
						{
							conv >> character.y_offset;
						}
						else if (key == "xadvance")
						{
							conv >> character.x_advance;
						}
					});
				chars.emplace_back(character);
			}
			else if(lineType == "kerning")
			{
				Kerning kerning;
				forEveryKeyValuePair(line, [this, &kerning](const string& key, const string& value)
					{
						std::stringstream conv;
						conv << value;
						if (key == "first")
						{
							conv >> kerning.first;
						}
						else if (key == "second")
						{
							conv >> kerning.second;
						}
						else if (key == "amount")
						{
							conv >> kerning.amount;
						}
					});
				kernings.emplace_back(kerning);
			}
		}
	}
	void forEveryKeyValuePair(const string& line, std::function<void(const string& key, const string& value)> func)
	{
		std::stringstream lineStream;
		lineStream << line;
		while (!lineStream.eof())
		{
			std::stringstream conv;
			string next;
			lineStream >> next;
			auto index = next.find('=');
			if (index == string::npos) continue;
			string key = next.substr(0, index);
			string value = next.substr(index + 1);

			if (value[0] == '\"' && value[value.size() - 1] == '\"')
			{
				value = value.substr(1, value.size() - 2);
			}
			func(key, value);
		}
	}
};


static FontKerning s_zero_kerning;
static FontCharacter s_zero_character;

Font::Font(const string& file) : _line_height(0)
{
	if (!file::exists(file))
	{
		log().error(string_format("font file %s does not exists", file));
		assert(0);
		return;
	}

	//bmfont
	if (path::extension(file) == ".fnt")
	{
		BMFont bmfont(file);
		if (bmfont.commmon.pages > 1)
		{
			log().message("Only 1 page bmfonts are supported");
			assert(0);
		}
		for (auto& it : bmfont.chars)
		{
			FontCharacter fc;
			fc.glyph = it.id;
			fc.width = it.width;
			fc.height = it.height;
			fc.x = it.x;
			fc.x_advance = it.x_advance;
			fc.x_offset = it.x_offset;
			fc.y = it.y;
			fc.y_offset = it.y_offset;
			_characters.emplace(std::make_pair(it.id, fc));
		}
		for (auto& it : bmfont.kernings)
		{
			FontKerning kerning;
			kerning.amount = it.amount;
			kerning.first = it.first;
			kerning.second = it.second;
			_kerning.emplace_back(kerning);
		}

		string filePath = path::withoutFile(file);
		string imagePath = path::combine(filePath, bmfont.pages[0].file);
		_image.load(imagePath);
		_line_height = bmfont.commmon.line_height;
	}
}

const map<u32, FontCharacter>& Font::characters() const
{
	return _characters;
}

const vector<FontKerning>& Font::kernings() const
{
	return _kerning;
}

const string& Font::name() const
{
	return _name;
}

const Image& Font::image() const
{
	return _image;
}

u16 Font::lineHeight() const
{
	return _line_height;
}
