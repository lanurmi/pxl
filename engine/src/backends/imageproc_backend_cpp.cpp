#include <pxl/backends/imageproc_backend.h>
#include <pxl/math/calc.h>

namespace
{

	void boxfilterrgba(const pxl::u8* in, pxl::u8* out, int w, int h, int x, int y, int size)
	{
		assert(size % 2 == 1);
		int s = (size - 1) / 2;
		// 0 ->
		// 1 ->
		int sums[4] = { 0,0,0,0 };

		int minx = pxl::calc::max(x - s, 0);
		int maxx = pxl::calc::min(x + s, w-1);

		int miny = pxl::calc::max(y - s, 0);
		int maxy = pxl::calc::min(y + s, h-1);

		int rows = (maxy - miny + 1);
		int cols = (maxx - minx + 1);

		for (int j = miny; j <= maxy; j++)
		{
			for (int i = minx; i <= maxx; i++)
			{
				const auto idx = (j * w + i) * 4;
				sums[0] += in[idx];
				sums[1] += in[idx + 1];
				sums[2] += in[idx + 2];
				sums[3] += in[idx + 3];

			}
		}

		int trueSize = (maxx - minx + 1) * (maxy - miny + 1);

		{
			out[(y * w + x) * 4] = static_cast<pxl::u8>(sums[0] / trueSize);
			out[(y * w + x) * 4 + 1] = static_cast<pxl::u8>(sums[1] / trueSize);
			out[(y * w + x) * 4 + 2] = static_cast<pxl::u8>(sums[2] / trueSize);
			out[(y * w + x) * 4 + 3] = static_cast<pxl::u8>(sums[3] / trueSize);
		}

	}

}

// Super naive and unoptimized blur
void pxl::imageproc::boxfilter(const pxl::Image& src, pxl::Image& dst, const pxl::Rect& rect, int size)
{
	assert(src.width() == dst.width());
	assert(src.height() == dst.height());
	assert(size % 2 == 1);
	auto srcPtr = &src.pixels()->r;
	auto dstPtr = &dst.pixels()->r;
	for (int j = rect.top(); j < rect.bottom(); j++)
	{
		for (int i = rect.left(); i < rect.right(); i++)
		{
			boxfilterrgba(srcPtr, dstPtr, src.width(), src.height(), i, j, size);
		}
	}
}

