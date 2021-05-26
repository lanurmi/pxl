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


Camera::Camera(int width, int height, int tilesize) : _size(width, height), _tilesize(tilesize), _transitioning(false), target(nullptr)
{
}

void Camera::awake()
{
	setUpdateOrder(update_order);
}

void Camera::transition(const Rect& rect, Transition transition)
{
	auto cam = get<Camera>();
	_bounds = rect;
	Vec2 useSize = cam->_size;
	useSize.x += (int)cam->_size.x % _tilesize;
	useSize.y += (int)cam->_size.y % _tilesize;

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

const Mat3x2 Camera::matrix() const
{
	int x = (entity()->position.x - _size.x / 2);
	int y = (entity()->position.y - _size.y / 2);

	return Mat3x2::createTranslation(Vec2(-x, -y));
}

Rect Camera::bounds() const
{
	int x = (entity()->position.x - _size.x / 2);
	int y = (entity()->position.y - _size.y / 2);
	return Rect(x, y, _size.x, _size.y);
}

bool Camera::transitioning() const
{
	return _slide_tween.running();
}

void Camera::update()
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
		Vec2 useSize = cam->_size;
		useSize.x += (int)cam->_size.x % _tilesize;
		useSize.y += (int)cam->_size.y % _tilesize;
		auto p = getPosition(useSize, _bounds, target->position);
		entity()->position = p;
	}
}