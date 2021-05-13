#pragma once
#include <pxl/math/color.h>
#include <pxl/image/image.h>

namespace pxl
{
	class ImagePacker
	{
	public:
		struct Entry
		{
			u32 id;
			int width;
			int height;
			int x;
			int y;
		};
		struct Result
		{
			Image image;
			std::vector<Entry> entries;
		};
		ImagePacker(int width, int height);
		~ImagePacker();
		void add(u32 id, const Image &image);
		void add(u32 id, int width, int height, const Color *pixels);
		const std::vector<Result>& pack();
	private:
		int doPack(int fromEntry);
		struct Buffer
		{
			u32 id;
			Color* pixels;
		};
		int _width;
		int _height;
		
		std::vector<Entry> _entries;
		std::vector<Buffer> _pixels;
		std::vector<Result> _result;
	};
}