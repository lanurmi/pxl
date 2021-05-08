#pragma once

#include <pxl/math/vec2.h>

namespace pxl
{
	struct Mat3x2;

	struct Rect
	{
		Rect();
		Rect(float rx, float ry, float rw, float rh);
		Rect(const Vec2& pos, const Vec2& size);
		bool operator==(const Rect& rhs) const;
		bool operator!=(const Rect& rhs) const;
		Rect operator+(const Vec2& rhs) const;
		Rect operator-(const Vec2& rhs) const;
		Rect& operator+=(const Vec2& rhs);
		Rect& operator-=(const Vec2& rhs);
		float x;
		float y;
		float width;
		float height;
		Rect scale(float s);
		Rect scale(float sx, float sy);
		float left() const;
		float right() const;
		float top() const;
		float bottom() const;
		Vec2 center() const;
		float centerX() const;
		float centerY() const;
		Vec2 topLeft() const;
		Vec2 topRight() const;
		Vec2 bottomRight() const;
		Vec2 bottomLeft() const;
		Vec2 centerLeft() const;
		Vec2 centerRight() const;
		Vec2 middleTop() const;
		Vec2 middleBottom() const;
		Vec2 size() const;
		bool contains(const Vec2& pt) const;
		bool overlaps(const Rect& rectangle) const;
		Rect inflate(float amount) const;
		static Rect transform(const Rect& vec, const Mat3x2& matrix);
	};
}