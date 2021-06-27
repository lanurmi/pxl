#pragma once

#include <pxl/utils/component.h>
#include <pxl/math/mat3x2.h>
#include <pxl/math/rect.h>
#include <pxl/math/color.h>
#include <pxl/math/tween.h>

namespace pxl
{
	class Camera : public Component
	{
	public:

		Camera() = default;
		Camera(int width, int height, int tilesize);
		const Mat3x2 matrix() const;
		Vec2 size() const;
		Rect view() const;
	private:
		Vec2 _size;
	};

	class PlatformerCameraController : public Component, public IUpdateable
	{
	public:
		enum class Transition
		{
			Instant,
			Slide
		};
		PlatformerCameraController();
		i16 updateOrder() const override;
		const Entity* target;
		Rect bounds() const;
		void transition(const Rect& rect, Transition transition);
		bool transitioning() const;
		void update() override;
	private:
		Vec2Tween _slide_tween;
		Rect _bounds;
	};
}