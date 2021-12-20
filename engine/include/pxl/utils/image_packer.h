#pragma once
#include <pxl/math/color.h>
#include <pxl/assets/image.h>

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
		const Vector<Result>& pack();
		void clear();
	private:
		int doPack(int fromEntry);
		struct Buffer
		{
			u32 id;
			Color* pixels;
		};
		int _width;
		int _height;
		
		Vector<Entry> _entries;
		Vector<Buffer> _pixels;
		Vector<Result> _result;
	};
}

