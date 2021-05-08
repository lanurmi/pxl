#pragma once
#include <cmath>

namespace pxl
{
	struct Vec3
	{
		float x;
		float y;
		float z;

		Vec3();
		Vec3(float x, float y, float z);
		Vec3 operator+(const Vec3& rhs) const;
		Vec3 operator-(const Vec3& rhs) const;
		Vec3 normal() const;
		static float dot(const Vec3& a, const Vec3& b);
		static Vec3 cross(const Vec3& a, const Vec3& b);
	};
}