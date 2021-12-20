#include <pxl/math/Vec3.h>

pxl::Vec3::Vec3() : x(0), y(0), z(0)
{
}

pxl::Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z)
{
}

pxl::Vec3 pxl::Vec3::operator+(const pxl::Vec3& rhs) const
{
	return pxl::Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

pxl::Vec3 pxl::Vec3::operator-(const pxl::Vec3& rhs) const
{
	return pxl::Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

pxl::Vec3 pxl::Vec3::normal() const
{
	float ls = x * x + y * y + z * z;
	float length = (float)sqrt(ls);
	return pxl::Vec3(x / length, y / length, z / length);
}

float pxl::Vec3::dot(const pxl::Vec3& a, const pxl::Vec3& b)
{
	return a.x * b.x +
		a.y * b.y +
		a.z * b.z;
}

pxl::Vec3 pxl::Vec3::cross(const pxl::Vec3& a, const pxl::Vec3& b)
{
	return pxl::Vec3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

