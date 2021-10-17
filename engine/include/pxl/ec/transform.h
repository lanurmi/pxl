#pragma once

#include <pxl/math/vec2.h>

namespace pxl
{
	class Transform
	{
	public:
		Vec2 position = Vec2::zero;
		Vec2 scale = Vec2::one;
	};
}