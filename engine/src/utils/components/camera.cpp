#include <pxl/utils/components/camera.h>
#include <pxl/math/vec2.h>
#include <pxl/utils/entity.h>
#include <pxl/graphics/batch.h>

using namespace pxl;

Camera::Camera(int width, int height) : size(width, height)
{
}

const Mat3x2 Camera::matrix() const
{
	int x = (entity()->position.x - size.x / 2);
	int y = (entity()->position.y - size.y / 2);

	return Mat3x2::createTranslation(Vec2(-x, -y));
}