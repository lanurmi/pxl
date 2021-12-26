#pragma once
#include <pxl/math/vec2.h>

namespace pxl
{
	class PlatformBackend;

	constexpr int s_max_mouse_buttons = 16;
	enum class MouseButton
	{
		None = -1,
		Left = 0,
		Middle = 1,
		Right = 2,
	};

	namespace mouse
	{
		bool pressed(MouseButton button);
		bool down(MouseButton button);
		bool released(MouseButton button);
		Vec2 position();
		Vec2 localPosition();
		Vec2 drawPosition();
		int wheelx();
		int wheely();

		void onButtonDown(MouseButton key); // called by platform
		void onButtonUp(MouseButton key); // called by platform
		void onMousePosition(const pxl::Vec2& position); // called by platform
		void onWheel(int x, int y); // called by platform;
	};
}

