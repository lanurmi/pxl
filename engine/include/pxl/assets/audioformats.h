#pragma once

#include <pxl/utils/filestream.h>

namespace pxl
{
	namespace audioformats
	{
		short *decode(FileStream& stream, int &size, int &samples, int &channels);
	}
}
