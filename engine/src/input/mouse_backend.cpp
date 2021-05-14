#include <pxl/input/mouse_backend.h>
#include <pxl/engine.h>
#include <pxl/types.h>


class MouseState
{
public:
	bool pressed[pxl::s_max_mouse_buttons];
	bool down[pxl::s_max_mouse_buttons];
	bool released[pxl::s_max_mouse_buttons];
	pxl::u64 timestamp[pxl::s_max_mouse_buttons];
	pxl::Vec2 position;
	pxl::Vec2 draw_position;
};

static MouseState s_mouseState;
static MouseState s_nextMouseState;

bool pxl::MouseBackend::pressed(MouseButton button)
{
	int i = (int)button;
	return i < pxl::s_max_mouse_buttons && s_mouseState.pressed[i];
}

bool pxl::MouseBackend::down(MouseButton button)
{
	int i = (int)button;
	return i < pxl::s_max_mouse_buttons&& s_mouseState.down[i];
}

bool pxl::MouseBackend::released(MouseButton button)
{
	int i = (int)button;
	return i < pxl::s_max_mouse_buttons&& s_mouseState.released[i];
}

pxl::Vec2 pxl::MouseBackend::position()
{
	return s_mouseState.position;
}

pxl::Vec2 pxl::MouseBackend::drawPosition()
{
	return s_mouseState.draw_position;
}

void pxl::MouseBackend::onButtonDown(MouseButton button)
{
	int i = (int)button;
	if (i < pxl::s_max_mouse_buttons)
	{
		s_nextMouseState.down[i] = true;
		s_nextMouseState.pressed[i] = true;
	}
}

void pxl::MouseBackend::onButtonUp(MouseButton button)
{
	int i = (int)button;
	if (i < pxl::s_max_mouse_buttons)
	{
		s_nextMouseState.released[i] = true;
	}
}

void pxl::MouseBackend::onMousePosition(const pxl::Vec2& position)
{
	s_nextMouseState.position = position;
	auto size = pxl::engine().size();
	auto drawSize = pxl::engine().drawSize();
	s_nextMouseState.draw_position = pxl::Vec2(position.x / static_cast<float>(size.x) * drawSize.x, position.y / static_cast<float>(size.y) * drawSize.y);
}


void pxl::MouseBackend::update()
{
	s_mouseState = s_nextMouseState;

	for (int i = 0; i < pxl::s_max_mouse_buttons; i++)
	{
		s_nextMouseState.pressed[i] = false;
		s_nextMouseState.released[i] = false;
	}
}
