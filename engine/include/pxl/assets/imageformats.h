#pragma once

#include <pxl/utils/filestream.h>
#include <pxl/math/color.h>

namespace pxl
{
	namespace imageformats
	{
		pxl::Color* decode(FileStream& stream, int* w, int* h);
	}
}