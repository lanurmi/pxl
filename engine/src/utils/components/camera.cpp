#include <pxl/utils/components/camera.h>
#include <pxl/math/vec2.h>
#include <pxl/utils/entity.h>
#include <pxl/graphics/batch.h>
#include <pxl/math/calc.h>
#include <pxl/time.h>

using namespace pxl;

namespace
{
	constexpr float transition_speed = 500.0f;
	constexpr u16 update_order = 9999;
}


Camera::Camera(int width, int height, int tilesize) : _size(width, height)
{
}


const Mat3x2 Camera::matrix() const
{
	int x = (entity()->position.x - _size.x / 2);
	int y = (entity()->position.y - _size.y / 2);

	return Mat3x2::createTranslation(Vec2(-x, -y));
}

Vec2 Camera::size() const
{
	return _size;
}


static Vec2 getPosition(const Vec2& cameraSize, const Rect& bounds, const Vec2 targetPos)
{
	Vec2 p = Vec2::zero;
	if (bounds.width > cameraSize.x) {
		p.x = calc::clamp(targetPos.x, bounds.left() + cameraSize.x / 2, bounds.right() - cameraSize.x / 2);
	}
	else {
		p.x = bounds.center().x;
	}

	if (bounds.height > cameraSize.y) {
		p.y = calc::clamp(targetPos.y, bounds.top() + cameraSize.y / 2, bounds.bottom() - cameraSize.y / 2);
	}
	else {
		p.y = bounds.center().y;
	}

	return p;
}

PlatformerCameraController::PlatformerCameraController() : target(nullptr)
{

}

i16 PlatformerCameraController::updateOrder() const
{
	return update_order;
}

void PlatformerCameraController::transition(const Rect& rect, Transition transition)
{
	auto cam = get<Camera>();
	_bounds = rect;
	Vec2 useSize = cam->size();
	useSize.x += (int)useSize.x % 16;
	useSize.y += (int)useSize.y % 16;

	auto p = getPosition(useSize, _bounds, target->position);
	if (transition == Transition::Slide)
	{
		_slide_tween.start(entity()->position, p, 0.5f, Easing::QuadOut);
	}
	else
	{
		entity()->position = p;
	}
}

bool PlatformerCameraController::transitioning() const
{
	return _slide_tween.running();
}

void PlatformerCameraController::update()
{
	if (target == nullptr) return;

	if (_slide_tween.running()) {
		_slide_tween.update(time::unscaled_delta);
		auto value = _slide_tween.value();
		entity()->position.x = (int)value.x;
		entity()->position.y = (int)value.y;
	}
	else
	{
		auto cam = get<Camera>();
		Vec2 useSize = cam->size();
		useSize.x += (int)useSize.x % 16;
		useSize.y += (int)useSize.y % 16;
		auto p = getPosition(useSize, _bounds, target->position);
		entity()->position = p;
	}
}