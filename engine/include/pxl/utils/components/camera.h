#pragma once

#include <pxl/utils/component.h>
#include <pxl/math/mat3x2.h>
#include <pxl/math/rect.h>
#include <pxl/math/color.h>
#include <pxl/math/tween.h>

namespace pxl
{
	class Camera : public Component, public IUpdateable
	{
	public:
		enum class Transition
		{
			Instant,
			Slide
		};
		Camera() = default;
		Camera(int width, int height, int tilesize);
		void awake() override;
		const Entity* target;
		const Mat3x2 matrix() const;
		Rect bounds() const;
		void transition(const Rect& rect, Transition transition);
		bool transitioning() const;
		void update() override;
	private:
		Vec2Tween _slide_tween;
		Vec2 _size;
		int _tilesize;
		Rect _bounds;
		bool _transitioning;
		Vec2 _transition_target;
		Vec2 _transition_now;
	};
}