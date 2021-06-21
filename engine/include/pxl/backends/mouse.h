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

	class Mouse
	{
	public:
		//
		bool pressed(MouseButton button);
		bool down(MouseButton button);
		bool released(MouseButton button);
		Vec2 position();
		Vec2 drawPosition();
	private:
		void update();
		void onButtonDown(MouseButton key);
		void onButtonUp(MouseButton key);
		void onMousePosition(const pxl::Vec2& position);
		friend class PlatformBackend;
	};
}