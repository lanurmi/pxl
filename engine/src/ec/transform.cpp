#include <pxl/ec/transform.h>
#include <pxl/math/calc.h>

using namespace pxl;

Mat3x2 Transform::matrix() const
{
	return Mat3x2::createTransform(position, pxl::Vec2::zero, scale, rotation);
}
