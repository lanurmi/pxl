#pragma once
#include <pxl/assets/image.h>
#include <pxl/math/rect.h>

namespace pxl
{
	namespace imageproc
	{
		void boxfilter(const Image& src, Image& dst, const Rect& rect, int size);
	}
}

