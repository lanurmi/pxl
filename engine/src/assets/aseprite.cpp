#include <pxl/assets/aseprite.h>
#include <pxl/math/calc.h>
#define STBI_NO_STDIO
#define STBI_ONLY_ZLIB
#include <pxl/3rdparty/stb_image.h>
#include <assert.h>
using namespace pxl;
using namespace file;

Aseprite::Aseprite() = default;

Aseprite::Aseprite(const String & path)
{
	FileStream fs(path, FileMode::ReadBinary);
	parse(fs);
}

Aseprite::Aseprite(const Aseprite & src)
{
	mode = src.mode;
	width = src.width;
	height = src.height;
	layers = src.layers;
	frames = src.frames;
	tags = src.tags;
	slices = src.slices;
	palette = src.palette;
}

Aseprite::Aseprite(Aseprite && src) noexcept
{
	mode = src.mode;
	width = src.width;
	height = src.height;
	layers = std::move(src.layers);
	frames = std::move(src.frames);
	tags = std::move(src.tags);
	slices = std::move(src.slices);
	palette = std::move(src.palette);
}

Aseprite& Aseprite::operator=(const Aseprite & src)
{
	Aseprite copy(src);
	std::swap(*this, copy);
	return *this;
}

Aseprite& Aseprite::operator=(Aseprite && src) noexcept
{
	mode = src.mode;
	width = src.width;
	height = src.height;
	layers = std::move(src.layers);
	frames = std::move(src.frames);
	tags = std::move(src.tags);
	slices = std::move(src.slices);
	palette = std::move(src.palette);
	return *this;
}

Aseprite::~Aseprite()
= default;

void Aseprite::parse(FileStream & stream)
{
	if (!stream.readable())
	{
		assert(0);
		return;
	}

	int frame_count = 0;

	// header
	{
		// filesize
		stream.read<u32>();

		// magic number
		auto magic = stream.read<u16>();
		if (magic != 0xA5E0)
		{
			assert(0 && "File is not a valid Aseprite file");
			return;
		}

		// main info
		frame_count = stream.read<u16>();
		width = stream.read<u16>();
		height = stream.read<u16>();
		mode = static_cast<Aseprite::Modes>(stream.read<u16>() / 8);

		// don't care about other info
		stream.read<u32>();		// Flags
		stream.read<u16>();		// Speed (deprecated)
		stream.read<u32>();		// Should be 0
		stream.read<u32>();		// Should be 0
		stream.read<u8>();		// Palette entry
		stream.seek(stream.position() + 3);			// Ignore these bytes
		stream.read<u16>();		// Number of colors (0 means 256 for old sprites)
		stream.read<i8>();		// Pixel width
		stream.read<i8>();		// Pixel height
		stream.seek(stream.position() + 92);		// For Future
	}

	frames.resize(frame_count);

	// frames
	for (int i = 0; i < frame_count; i++)
	{
		auto frameStart = stream.position();
		auto frameEnd = frameStart + stream.read<u32>();
		unsigned int chunks = 0;

		// frame header
		{
			auto magic = stream.read<u16>(); // magic number
			if (magic != 0xF1FA)
			{
				assert(0 && "File is not a valid Aseprite file");
				return;
			}

			auto old_chunk_count = stream.read<u16>();
			frames[i].duration = stream.read<u16>();
			stream.seek(stream.position() + 2); // for future
			auto new_chunk_count = stream.read<u32>();

			if (old_chunk_count == 0xFFFF)
				chunks = new_chunk_count;
			else
				chunks = old_chunk_count;
		}

		// make frame image
		frames[i].image = Image(width, height);

		// frame chunks
		for (unsigned int j = 0; j < chunks; j++)
		{
			auto chunkStart = stream.position();
			auto chunkEnd = chunkStart + stream.read<u32>();
			auto chunkType = static_cast<Chunks>(stream.read<u16>());

			switch (chunkType)
			{
				case Chunks::Layer: parse_layer(stream, i); break;
				case Chunks::Cel: parse_cel(stream, i, chunkEnd); break;
				case Chunks::Palette: parse_palette(stream, i); break;
				case Chunks::UserData: parse_user_data(stream, i); break;
				case Chunks::FrameTags: parse_tag(stream, i); break;
				case Chunks::Slice: parse_slice(stream, i); break;
				default: break;
			}

			stream.seek(chunkEnd);
		}

		stream.seek(frameEnd);
	}
}

void Aseprite::parse_layer(FileStream & stream, int frame)
{
	layers.push_back(pxl::Aseprite::Layer());

	auto& layer = layers.back();
	layer.flag = static_cast<LayerFlags>(stream.read<u16>());
	layer.visible = ((int)layer.flag & (int)LayerFlags::Visible) == (int)LayerFlags::Visible;
	layer.type = static_cast<LayerTypes>(stream.read<u16>());
	layer.child_level = stream.read<u16>();
	stream.read<u16>(); // width
	stream.read<u16>(); // height
	layer.blendmode = stream.read<u16>();
	layer.alpha = stream.read<u8>();
	stream.seek(stream.position() + 3); // for future

	auto size = stream.read<u16>();
	layer.name.resize(size);
	stream.read((u8*)layer.name.data(), layer.name.size());

	layer.userdata.color = 0xffffff;
	layer.userdata.text = "";
	m_last_userdata = &(layer.userdata);
}

void Aseprite::parse_cel(FileStream & stream, int frameIndex, size_t maxPosition)
{
	Frame& frame = frames[frameIndex];

	frame.cels.push_back(pxl::Aseprite::Cel());
	auto& cel = frame.cels.back();
	cel.layer_index = stream.read<u16>();
	cel.x = stream.read<u16>();
	cel.y = stream.read<u16>();
	cel.alpha = stream.read<u8>();
	cel.linked_frame_index = -1;

	auto celType = stream.read<u16>();
	stream.seek(stream.position() + 7);

	// RAW or DEFLATE
	if (celType == 0 || celType == 2)
	{
		auto width = stream.read<u16>();
		auto height = stream.read<u16>();
		auto count = width * height * (int)mode;

		cel.image = Image(width, height);

		// RAW
		if (celType == 0)
		{
			stream.read((u8*)cel.image.pixels(), count);
		}
		// DEFLATE (zlib)
		else
		{
			// this could be optimized to use a buffer on the stack if we only read set chunks at a time
			// stbi's zlib doesn't have that functionality though
			auto size = maxPosition - stream.position();
			if (size > INT32_MAX)
				size = INT32_MAX;

			char* buffer = new char[size];
			stream.read((u8*)buffer, size);

			int olen = width * height * sizeof(Color);
			int res = stbi_zlib_decode_buffer((char*)cel.image.pixels(), olen, buffer, (int)size);

			delete[] buffer;

			if (res < 0)
			{
				assert(0 && "Unable to parse Aseprite file");
				return;
			}
		}

		// convert to pixels
		// note: we work in-place to save having to store stuff in a buffer
		if (mode == Modes::Grayscale)
		{
			auto src = (unsigned char*)cel.image.pixels();
			auto dst = cel.image.pixels();
			for (int d = width * height - 1, s = (width * height - 1) * 2; d >= 0; d--, s -= 2)
				dst[d] = Color(src[s], src[s], src[s], src[s + 1]);
		}
		else if (mode == Modes::Indexed)
		{
			auto src = (unsigned char*)cel.image.pixels();
			auto dst = cel.image.pixels();
			for (int i = width * height - 1; i >= 0; i--)
				dst[i] = palette[src[i]];
		}

	}
	// REFERENCE
	// this cel directly references a previous cel
	else if (celType == 1)
	{
		cel.linked_frame_index = stream.read<u16>();
	}

	// draw to frame if visible
	if ((int)layers[cel.layer_index].flag & (int)LayerFlags::Visible)
	{
		render_cel(&cel, &frame);
	}

	cel.userdata.color = 0xffffff;
	cel.userdata.text = "";
	m_last_userdata = &(cel.userdata);
}

void Aseprite::parse_palette(FileStream & stream, int frame)
{
	/* size */ stream.read<u32>();
	auto start = stream.read<u32>();
	auto end = stream.read<u32>();
	stream.seek(stream.position() + 8);

	palette.resize(palette.size() + (end - start + 1));

	for (int p = 0, len = static_cast<int>(end - start) + 1; p < len; p++)
	{
		auto hasName = stream.read<u16>();
		palette[start + p] = stream.read<u32>();

		if (hasName & 0xF000)
		{
			int len = stream.read<u16>();
			stream.seek(stream.position() + len);
		}
	}
}

void Aseprite::parse_user_data(FileStream & stream, int frame)
{
	if (m_last_userdata != nullptr)
	{
		auto flags = stream.read<u32>();

		// has text
		if (flags & (1 << 0))
		{
			auto size = stream.read<u16>();
			m_last_userdata->text.resize(size);
			stream.read((u8*)m_last_userdata->text.data(), m_last_userdata->text.size());
		}

		// has color
		if (flags & (1 << 1))
			m_last_userdata->color = stream.read<u32>();
	}
}

void Aseprite::parse_tag(FileStream & stream, int frame)
{
	auto count = stream.read<u16>();
	stream.seek(stream.position() + 8);

	for (int t = 0; t < count; t++)
	{
		Tag tag;
		tag.from = stream.read<u16>();
		tag.to = stream.read<u16>();
		tag.loops = static_cast<LoopDirections>(stream.read<i8>());

		stream.seek(stream.position() + 8);
		tag.color = Color(stream.read<i8>(), stream.read<i8>(), stream.read<i8>(), 255);
		stream.seek(stream.position() + 1);

		
		auto s = stream.read<u16>();
		tag.name.resize(s); 
		stream.read((u8*)tag.name.data(), tag.name.size());

		tags.push_back(tag);
	}
}

void Aseprite::parse_slice(FileStream & stream, int frame)
{
	int count = stream.read<u32>();
	int flags = stream.read<u32>();
	stream.read<u32>(); // reserved

	String name;
	auto size = stream.read<u16>();
	name.resize(size);
	stream.read((u8*)name.data(), name.size());

	for (int s = 0; s < count; s++)
	{
		slices.push_back(Aseprite::Slice());

		auto& slice = slices.back();
		slice.name = name;
		slice.frame = stream.read<u32>();
		slice.origin.x = stream.read<i32>();
		slice.origin.y = stream.read<i32>();
		slice.width = stream.read<u32>();
		slice.height = stream.read<u32>();

		// 9 slice (ignored atm)
		if (flags & (1 << 0))
		{
			stream.read<i32>();
			stream.read<i32>();
			stream.read<u32>();
			stream.read<u32>();
		}

		// pivot point
		slice.has_pivot = false;
		if (flags & (1 << 1))
		{
			slice.has_pivot = true;
			slice.pivot.x = stream.read<u32>();
			slice.pivot.y = stream.read<u32>();
		}

		slice.userdata.color = 0xffffff;
		slice.userdata.text = "";
		m_last_userdata = &(slice.userdata);
	}
}

#define MUL_UN8(a, b, t) \
	((t) = (a) * (u16)(b) + 0x80, ((((t) >> 8) + (t) ) >> 8))

void Aseprite::render_cel(Cel * cel, Frame * frame)
{
	Layer& layer = layers[cel->layer_index];

	while (cel->linked_frame_index >= 0)
	{
		auto& frame = frames[cel->linked_frame_index];
		for (auto& it : frame.cels)
			if (it.layer_index == cel->layer_index)
			{
				cel = &it;
				break;
			}
	}

	int t;
	unsigned char opacity = MUL_UN8(cel->alpha, layer.alpha, t);
	if (opacity <= 0)
		return;

	auto src = cel->image.pixels();
	auto srcX = cel->x;
	auto srcY = cel->y;
	auto srcW = cel->image.width();
	auto srcH = cel->image.height();
	auto dst = frame->image.pixels();
	auto dstW = frame->image.width();
	auto dstH = frame->image.height();

	// blit pixels
	int left = calc::max(0, srcX);
	int right = calc::min(dstW, srcX + srcW);
	int top = calc::max(0, srcY);
	int bottom = calc::min(dstH, srcY + srcH);

	if (layer.blendmode == 0)
	{
		for (int dx = left, sx = -calc::min(srcX, 0); dx < right; dx++, sx++)
		{
			for (int dy = top, sy = -calc::min(srcY, 0); dy < bottom; dy++, sy++)
			{
				Color* srcColor = (src + sx + sy * srcW);
				Color* dstColor = (dst + dx + dy * dstW);

				if (srcColor->a != 0)
				{
					auto sa = MUL_UN8(srcColor->a, opacity, t);
					auto ra = dstColor->a + sa - MUL_UN8(dstColor->a, sa, t);

					dstColor->r = (unsigned char)(dstColor->r + (srcColor->r - dstColor->r) * sa / ra);
					dstColor->g = (unsigned char)(dstColor->g + (srcColor->g - dstColor->g) * sa / ra);
					dstColor->b = (unsigned char)(dstColor->b + (srcColor->b - dstColor->b) * sa / ra);
					dstColor->a = (unsigned char)ra;
				}
			}
		}
	}
	else
	{
		assert(0 && "Aseprite blendmodes aren't implemented");
	}
}