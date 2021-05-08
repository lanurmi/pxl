#pragma once

namespace pxl
{
	struct Vec4
	{
		float x;
		float y;
		float z;
		float width;
		Vec4() : x(0), y(0), z(0), width(0) {}
		Vec4(float x, float y, float z, float width) : x(x), y(y), z(z), width(width) {}
	};
}
