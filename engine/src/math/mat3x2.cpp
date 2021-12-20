#include <pxl/math/mat3x2.h>
#include <pxl/math/Vec2.h>
#include <cmath>
#include <cstring>

pxl::Mat3x2::Mat3x2()
	: m11(0), m12(0), m21(0), m22(0), m31(0), m32(0)
{
}

pxl::Mat3x2::Mat3x2(float m11, float m12, float m21, float m22, float m31, float m32)
	: m11(m11), m12(m12), m21(m21), m22(m22), m31(m31), m32(m32)
{
}

pxl::Mat3x2 pxl::Mat3x2::invert() const
{
	auto det = (m11 * m22) - (m21 * m12);
	auto invDet = 1.0f / det;

	return pxl::Mat3x2
	{
		m22 * invDet,
		-m12 * invDet,
		-m21 * invDet,
		m11 * invDet,
		(m21 * m32 - m31 * m22) * invDet,
		(m31 * m12 - m11 * m32) * invDet
	};
}

float pxl::Mat3x2::getScalingFactor() const
{
	return (float)sqrt((double)m11 * m11 + (double)m12 * m12);
}

const pxl::Mat3x2 pxl::Mat3x2::identity = pxl::Mat3x2(1, 0, 0, 1, 0, 0);

pxl::Mat3x2 pxl::Mat3x2::operator *(const pxl::Mat3x2& rhs) const { return multiply(*this, rhs); }
pxl::Mat3x2 pxl::Mat3x2::operator +(const pxl::Mat3x2& rhs) const { return add(*this, rhs); }
pxl::Mat3x2 pxl::Mat3x2::operator -(const pxl::Mat3x2& rhs) const { return subtract(*this, rhs); }

pxl::Mat3x2& pxl::Mat3x2::operator*=(const pxl::Mat3x2& rhs)
{
	*this = multiply(*this, rhs);
	return *this;
}

bool pxl::Mat3x2::operator ==(const pxl::Mat3x2& rhs)
{
	return memcmp(this, &rhs, sizeof(pxl::Mat3x2)) == 0;
}

bool pxl::Mat3x2::operator !=(const pxl::Mat3x2& rhs)
{
	return !(*this == rhs);
}

pxl::Mat3x2 pxl::Mat3x2::createTranslation(const Vec2& Vec2)
{
	return pxl::Mat3x2(1, 0, 0, 1, Vec2.x, Vec2.y);
}

pxl::Mat3x2 pxl::Mat3x2::createScale(Vec2 Vec2)
{
	return  pxl::Mat3x2(Vec2.x, 0, 0, Vec2.y, 0, 0);
}

pxl::Mat3x2 pxl::Mat3x2::createRotation(float radians)
{
	float c = cosf(radians);
	float s = sinf(radians);

	return pxl::Mat3x2(c, s, -s, c, 0, 0);
}

pxl::Mat3x2 pxl::Mat3x2::createTransform(const Vec2& position, const Vec2& origin, const Vec2& scale, float rotation)
{
	pxl::Mat3x2 matrix = identity;

	if (origin.x != 0 || origin.y != 0)
	{
		matrix = createTranslation(-origin);
	}
	if (scale.x != 1 || scale.y != 1)
	{
		matrix = matrix * createScale(scale);
	}
	if (rotation != 0)
	{
		matrix = matrix * createRotation(rotation);
	}
	if (position.x != 0 || position.y != 0)
	{
		matrix = matrix * createTranslation(position);
	}

	return matrix;
}

pxl::Mat3x2 pxl::Mat3x2::add(const pxl::Mat3x2& a, const pxl::Mat3x2& b)
{
	return pxl::Mat3x2(
		a.m11 + b.m11,
		a.m12 + b.m12,
		a.m21 + b.m21,
		a.m22 + b.m22,
		a.m31 + b.m31,
		a.m32 + b.m32);
}

pxl::Mat3x2 pxl::Mat3x2::subtract(const pxl::Mat3x2& a, const pxl::Mat3x2& b)
{
	return pxl::Mat3x2(
		a.m11 - b.m11,
		a.m12 - b.m12,
		a.m21 - b.m21,
		a.m22 - b.m22,
		a.m31 - b.m31,
		a.m32 - b.m32);
}

pxl::Mat3x2 pxl::Mat3x2::multiply(const pxl::Mat3x2& a, const pxl::Mat3x2& b)
{
	return pxl::Mat3x2(a.m11 * b.m11 + a.m12 * b.m21,
		a.m11 * b.m12 + a.m12 * b.m22,
		a.m21 * b.m11 + a.m22 * b.m21,
		a.m21 * b.m12 + a.m22 * b.m22,
		a.m31 * b.m11 + a.m32 * b.m21 + b.m31,
		a.m31 * b.m12 + a.m32 * b.m22 + b.m32);
}

