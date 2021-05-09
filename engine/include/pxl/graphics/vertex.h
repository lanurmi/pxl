#pragma once
#include <pxl/math/Vec2.h>
#include <pxl/math/color.h>

namespace pxl
{
	struct Vertex
	{
		Vertex() {}
		Vertex(const Vec2 pos, const Vec2& tex, const Color& color) : position(pos), texture_coordinate(tex), color(color)
		{
		}
		Vec2 position;
		Vec2 texture_coordinate;
		Color color;
	};
}