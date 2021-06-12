#pragma once
#include <pxl/math/color.h>
#include <pxl/math/vec2.h>
#include <pxl/assets/image.h>
#include <pxl/utils/filestream.h>

// Borrowed from the blah project: https://github.com/NoelFB/blah
namespace pxl
{
	// A simple Aseprite file parser.
	// This implementation does not support Aseprite blendmodes,
	// aside from the default blend mode.
	class Aseprite
	{
	public:

		enum class Modes
		{
			Indexed = 1,
			Grayscale = 2,
			RGBA = 4
		};

		enum class Chunks
		{
			OldPaletteA = 0x0004,
			OldPaletteB = 0x0011,
			Layer = 0x2004,
			Cel = 0x2005,
			CelExtra = 0x2006,
			Mask = 0x2016,
			Path = 0x2017,
			FrameTags = 0x2018,
			Palette = 0x2019,
			UserData = 0x2020,
			Slice = 0x2022
		};

		enum class LoopDirections
		{
			Forward = 0,
			Reverse = 1,
			PingPong = 2
		};

		enum class LayerFlags
		{
			Visible = 1,
			Editable = 2,
			LockMovement = 4,
			Background = 8,
			PreferLinkedCels = 16,
			Collapsed = 32,
			Reference = 64
		};

		enum class LayerTypes
		{
			Normal = 0,
			Group = 1
		};

		struct UserData
		{
			string text;
			Color color;
		};

		struct Layer;

		struct Cel
		{
			int layer_index = 0;
			int linked_frame_index = 0;
			int x = 0;
			int y = 0;
			unsigned char alpha = 0;
			Image image;
			UserData userdata;
		};

		struct Frame
		{
			int duration = 0;
			Image image;
			vector<Cel> cels;
		};

		struct Layer
		{
			LayerFlags flag = (LayerFlags)0;
			LayerTypes type = LayerTypes::Normal;
			string name;
			int child_level = 0;
			int blendmode = 0;
			unsigned char alpha = 0;
			bool visible = true;
			UserData userdata;
		};

		struct Tag
		{
			string name;
			LoopDirections loops = LoopDirections::Forward;
			int from = 0;
			int to = 0;
			Color color;
			UserData userdata;
		};

		struct Slice
		{
			int frame = 0;
			string name;
			Vec2 origin;
			int width = 0;
			int height = 0;
			bool has_pivot = false;
			Vec2 pivot;
			UserData userdata;
		};

		Modes mode = Modes::RGBA;
		int width = 0;
		int height = 0;

		vector<Layer> layers;
		vector<Frame> frames;
		vector<Tag>   tags;
		vector<Slice> slices;
		vector<Color> palette;

		Aseprite();
		Aseprite(const string& path);
		Aseprite(const Aseprite& src);
		Aseprite(Aseprite&& src) noexcept;
		Aseprite& operator=(const Aseprite& src);
		Aseprite& operator=(Aseprite&& src) noexcept;
		~Aseprite();

	private:
		UserData* m_last_userdata = nullptr;

		void parse(FileStream& stream);
		void parse_layer(FileStream& stream, int frame);
		void parse_cel(FileStream& stream, int frame, size_t maxPosition);
		void parse_palette(FileStream& stream, int frame);
		void parse_user_data(FileStream& stream, int frame);
		void parse_tag(FileStream& stream, int frame);
		void parse_slice(FileStream& stream, int frame);
		void render_cel(Cel* cel, Frame* frame);
	};
}