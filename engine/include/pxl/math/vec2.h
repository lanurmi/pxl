#pragma once

namespace pxl
{
	struct Mat3x2;
	struct Mat4x4;
	struct Vec2
	{
		float x;
		float y;

		Vec2();
		Vec2(float vx, float vy);
		Vec2(int vx, int vy);

		Vec2 operator +(const Vec2& rhs) const;
		Vec2 operator -(const Vec2& rhs) const;
		Vec2 operator /(const float rhs) const;
		Vec2 operator *(const float rhs) const;
		Vec2 operator *(const Vec2 rhs) const;
		Vec2 operator -() const;

		Vec2& operator +=(const Vec2& rhs);
		Vec2& operator -=(const Vec2& rhs);
		Vec2& operator /=(const Vec2& rhs);
		Vec2& operator *=(const Vec2& rhs);
		Vec2& operator /=(float rhs);
		Vec2& operator *=(float rhs);

		bool operator ==(const Vec2& rhs) const;
		bool operator !=(const Vec2& rhs) const;

		Vec2 normal() const;
		Vec2 turnRight() const;
		Vec2 turnLeft() const;
		float length() const;
		float lengthSquared() const;
		Vec2 perpendicular() const;
		float angle() const;

		static float dot(Vec2 a, Vec2 b);
		static Vec2 transform(const Vec2& vec, const Mat3x2& matrix);
		static Vec2 fromAngle(float radians, float length = 1.0f);
		static Vec2 lerp(Vec2 start, Vec2 end, float t);
		static Vec2 reflect(const Vec2& vector, const Vec2& normal);
		static Vec2 min(const Vec2& a, const Vec2& b);
		static Vec2 max(const Vec2& a, const Vec2& b);

		static const Vec2 unitX;
		static const Vec2 unitY;
		static const Vec2 right;
		static const Vec2 up;
		static const Vec2 down;
		static const Vec2 left;
		static const Vec2 zero;
		static const Vec2 one;
		static const Vec2 up_right;
		static const Vec2 up_left;
		static const Vec2 down_right;
		static const Vec2 down_left;
	};
}