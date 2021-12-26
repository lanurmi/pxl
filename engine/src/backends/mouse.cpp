#include <pxl/backends/mouse.h>
#include <pxl/engine.h>
#include <pxl/types.h>


class MouseState
{
public:
	bool down[pxl::s_max_mouse_buttons];
	pxl::u64 timestamp[pxl::s_max_mouse_buttons];
	pxl::Vec2 position;
	pxl::Vec2 drawPosition;
	int wheelx = 0;
	int wheely = 0;
};

static MouseState s_mouseState;

bool pxl::mouse::down(MouseButton button)
{
	int i = (int)button;
	return i < pxl::s_max_mouse_buttons&& s_mouseState.down[i];
}

pxl::Vec2 pxl::mouse::position()
{
	return s_mouseState.position;
}


pxl::Vec2 pxl::mouse::localPosition()
{
	return s_mouseState.position - pxl::platform::position();
}

int pxl::mouse::wheelx()
{
	return s_mouseState.wheelx;
}

int pxl::mouse::wheely()
{
	return s_mouseState.wheely;

}
pxl::Vec2 pxl::mouse::drawPosition()
{
	return s_mouseState.drawPosition;
}

void pxl::mouse::onButtonDown(MouseButton button)
{
	int i = (int)button;
	if (i < pxl::s_max_mouse_buttons)
	{
		s_mouseState.down[i] = true;
	}
}

void pxl::mouse::onButtonUp(MouseButton button)
{
	int i = (int)button;
	if (i < pxl::s_max_mouse_buttons)
	{
		s_mouseState.down[i] = false;
	}
}

void pxl::mouse::onMousePosition(const pxl::Vec2& position)
{
	s_mouseState.position = position;
	auto size = pxl::platform::size();
	auto drawSize = pxl::platform::drawSize();
	auto windowPos = pxl::platform::position();
	s_mouseState.drawPosition = pxl::Vec2( (position.x - windowPos.x) / static_cast<float>(size.x) * drawSize.x,(position.y - windowPos.y) / static_cast<float>(size.y) * drawSize.y);
}

void pxl::mouse::onWheel(int x, int y) {
	s_mouseState.wheelx = x;
	s_mouseState.wheely = y;
}
