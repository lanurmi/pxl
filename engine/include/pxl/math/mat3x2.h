#pragma once

namespace pxl
{
	struct Vec2;

	struct Mat3x2
	{
		float m11;
		float m12;
		float m21;
		float m22;
		float m31;
		float m32;

		Mat3x2();
		Mat3x2(float m11, float m12, float m21, float m22, float m31, float m32);

		Mat3x2 invert() const;
		float getScalingFactor() const;

		Mat3x2 operator *(const Mat3x2& rhs) const;
		Mat3x2 operator +(const Mat3x2& rhs) const;
		Mat3x2 operator -(const Mat3x2& rhs) const;
		Mat3x2& operator *=(const Mat3x2& rhs);
		bool operator==(const Mat3x2& rhs);
		bool operator!=(const Mat3x2& rhs);

		static const Mat3x2 identity;
		static Mat3x2 createTranslation(const Vec2& position);
		static Mat3x2 createScale(Vec2 scale);
		static Mat3x2 createRotation(float radians);
		static Mat3x2 createTransform(const Vec2& position, const Vec2& origin, const Vec2& scale, float rotation);
		static Mat3x2 add(const Mat3x2& a, const Mat3x2& b);
		static Mat3x2 subtract(const Mat3x2& a, const Mat3x2& b);
		static Mat3x2 multiply(const Mat3x2& a, const Mat3x2& b);
	};
}

