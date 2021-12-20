#include <pxl/math/Vec2.h>
#include <pxl/math/mat3x2.h>
#include <cmath>


pxl::Vec2::Vec2() : x(0), y(0)
{
}

pxl::Vec2::Vec2(float vx, float vy) : x(vx), y(vy)
{
}

pxl::Vec2::Vec2(int vx, int vy) : x(static_cast<float>(vx)), y(static_cast<float>(vy))
{
}


pxl::Vec2 pxl::Vec2::operator +(const pxl::Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

pxl::Vec2 pxl::Vec2::operator -(const pxl::Vec2& rhs) const
{
	return pxl::Vec2(x - rhs.x, y - rhs.y);
}

pxl::Vec2 pxl::Vec2::operator /(const float rhs) const
{
	return pxl::Vec2(x / rhs, y / rhs);
}

pxl::Vec2 pxl::Vec2::operator *(const float rhs) const
{
	return Vec2(x * rhs, y * rhs);
}

pxl::Vec2 pxl::Vec2::operator *(const pxl::Vec2 rhs) const
{
	return pxl::Vec2(x * rhs.x, y * rhs.y);
}

pxl::Vec2 pxl::Vec2::operator-() const
{
	return Vec2(-x, -y);
}

pxl::Vec2& pxl::Vec2::operator +=(const pxl::Vec2& rhs)
{
	x += rhs.x; y += rhs.y; return *this;
}

pxl::Vec2& pxl::Vec2::operator -=(const pxl::Vec2& rhs)
{
	x -= rhs.x; y -= rhs.y; return *this;
}

pxl::Vec2& pxl::Vec2::operator /=(const pxl::Vec2& rhs)
{
	x /= rhs.x; y /= rhs.y; return *this;
}

pxl::Vec2& pxl::Vec2::operator *=(const pxl::Vec2& rhs)
{
	x *= rhs.x; y *= rhs.y; return *this;
}

pxl::Vec2& pxl::Vec2::operator/=(float rhs)
{
	x /= rhs; y /= rhs; return *this;
}

pxl::Vec2& pxl::Vec2::operator*=(float rhs)
{
	x *= rhs; y *= rhs; return *this;
}

bool pxl::Vec2::operator ==(const pxl::Vec2& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool pxl::Vec2::operator !=(const pxl::Vec2& rhs) const
{
	return x != rhs.x || y != rhs.y;
}

pxl::Vec2 pxl::Vec2::normal() const
{
	if (x == 0 && y == 0) return zero;
	float length = this->length();
	return pxl::Vec2(x / length, y / length);
}

pxl::Vec2 pxl::Vec2::turnRight() const
{
	return pxl::Vec2(y, -x);
}

pxl::Vec2 pxl::Vec2::turnLeft() const
{
	return pxl::Vec2(-y, x);
}

float pxl::Vec2::length() const
{
	return sqrtf(x * x + y * y);
}

float pxl::Vec2::lengthSquared() const
{
	return x * x + y * y;
}

pxl::Vec2 pxl::Vec2::perpendicular() const
{
	return pxl::Vec2(-y, x);
}

float pxl::Vec2::angle() const
{
	return atan2(y, x);
}

float pxl::Vec2::dot(pxl::Vec2 a, pxl::Vec2 b)
{
	return (a.x * b.x + a.y * b.y);
}

pxl::Vec2 pxl::Vec2::transform(const pxl::Vec2& vec, const Mat3x2& matrix)
{
	return pxl::Vec2(
		(vec.x * matrix.m11) + (vec.y * matrix.m21) + matrix.m31,
		(vec.x * matrix.m12) + (vec.y * matrix.m22) + matrix.m32);
}

pxl::Vec2 pxl::Vec2::fromAngle(float radians, float length)
{
	return pxl::Vec2((float)cos(radians) * length, (float)sin(radians) * length);
}

pxl::Vec2 pxl::Vec2::lerp(pxl::Vec2 a, pxl::Vec2 b, float t)
{
	if (t == 0)
	{
		return a;
	}
	else if (t == 1)
	{
		return b;
	}
	else
	{
		return a + (b - a) * t;
	}
}

pxl::Vec2 pxl::Vec2::reflect(const pxl::Vec2& vector, const pxl::Vec2& normal)
{
	float dot = vector.x * normal.x + vector.y * normal.y;

	return pxl::Vec2(
		vector.x - 2.0f * dot * normal.x,
		vector.y - 2.0f * dot * normal.y);
}

pxl::Vec2 pxl::Vec2::min(const pxl::Vec2& a, const pxl::Vec2& b)
{
	return pxl::Vec2(
		a.x < b.x ? a.x : b.x,
		a.y < b.y ? a.y : b.y);
}

pxl::Vec2 pxl::Vec2::max(const pxl::Vec2& a, const pxl::Vec2& b)
{
	return pxl::Vec2(
		a.x > b.x ? a.x : b.x,
		a.y > b.y ? a.y : b.y);
}

const pxl::Vec2 pxl::Vec2::unitX = pxl::Vec2(1, 0);
const pxl::Vec2 pxl::Vec2::unitY = pxl::Vec2(0, 1);
const pxl::Vec2 pxl::Vec2::right = pxl::Vec2(1, 0);
const pxl::Vec2 pxl::Vec2::up = pxl::Vec2(0, -1);
const pxl::Vec2 pxl::Vec2::down = pxl::Vec2(0, 1);
const pxl::Vec2 pxl::Vec2::left = pxl::Vec2(-1, 0);
const pxl::Vec2 pxl::Vec2::zero = pxl::Vec2(0, 0);
const pxl::Vec2 pxl::Vec2::one = pxl::Vec2(1, 1);

#define DIAG 0.70710678118f
const pxl::Vec2 pxl::Vec2::up_right = pxl::Vec2(DIAG, -DIAG);
const pxl::Vec2 pxl::Vec2::up_left = pxl::Vec2(-DIAG, -DIAG);
const pxl::Vec2 pxl::Vec2::down_right = pxl::Vec2(DIAG, DIAG);
const pxl::Vec2 pxl::Vec2::down_left = pxl::Vec2(-DIAG, DIAG);
#undef DIAG

