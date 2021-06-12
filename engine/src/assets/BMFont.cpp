#include <pxl/assets/BMFont.h>
#include <pxl/engine.h>
#include <pxl/utils/filestream.h>
#include <sstream>

using namespace pxl;

BMFont::BMFont(const string& file)
{
	load(file);
}


void BMFont::load(const string& file)
{
	FileStream fileRef(file, pxl::file::FileMode::Read);
	while (!fileRef.eof())
	{
		string line = fileRef.line();
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
		else if (lineType == "kerning")
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

void BMFont::forEveryKeyValuePair(const string& line, std::function<void(const string& key, const string& value)> func)
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
