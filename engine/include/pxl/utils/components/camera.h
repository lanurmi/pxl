#pragma once

#include <pxl/utils/component.h>
#include <pxl/math/mat3x2.h>
#include <pxl/graphics/render_target.h>
#include <pxl/graphics/batch.h>

namespace pxl
{
	class Camera : public Component
	{
	public:
		Camera() = default;
		Camera(int width, int height);
		Vec2 size;
		pxl::Color clear_color;
		const Mat3x2 matrix() const;
		Rect bounds() const;
	};
}