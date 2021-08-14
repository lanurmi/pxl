#include <pxl/backends/keyboard.h>
#include <pxl/time.h>

constexpr int s_max_keyboard_keys = 256;

class KeyboardState
{
public:

	bool pressed[s_max_keyboard_keys];
	bool down[s_max_keyboard_keys];
	bool released[s_max_keyboard_keys];
	pxl::u64 timestamp[s_max_keyboard_keys];
};

static KeyboardState s_keyboardState;
static KeyboardState s_nextKeyboardState;

void pxl::keyboard::update()
{
	s_keyboardState = s_nextKeyboardState;

	for (int i = 0; i < s_max_keyboard_keys; i++)
	{
		s_nextKeyboardState.pressed[i] = false;
		s_nextKeyboardState.released[i] = false;
	}
}

void pxl::keyboard::onKeyDown(Key key)
{
	int i = (int)key;
	if (i > 0 && i < s_max_keyboard_keys)
	{
		s_nextKeyboardState.down[i] = true;
		s_nextKeyboardState.pressed[i] = true;
		s_nextKeyboardState.timestamp[i] = pxl::time::ticks;
	}
}

void pxl::keyboard::onKeyUp(Key key)
{
	int i = (int)key;
	if (i > 0 && i < s_max_keyboard_keys)
	{
		s_nextKeyboardState.down[i] = false;
		s_nextKeyboardState.released[i] = true;
		s_nextKeyboardState.timestamp[i] = pxl::time::ticks;
	}
}

bool pxl::keyboard::pressed(Key key)
{
	int i = (int)key;
	return i > 0 && i < s_max_keyboard_keys&& s_keyboardState.pressed[i];
}

bool pxl::keyboard::down(Key key)
{
	int i = (int)key;
	return i > 0 && i < s_max_keyboard_keys&& s_keyboardState.down[i];
}

bool pxl::keyboard::released(Key key)
{
	int i = (int)key;
	return i > 0 && i < s_max_keyboard_keys && s_keyboardState.released[i];
}
