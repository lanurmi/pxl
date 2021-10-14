#include <pxl/backends/keyboard.h>
#include <pxl/time.h>

namespace
{
	constexpr int s_max_keyboard_keys = 256;

	class KeyboardState
	{
	public:

		bool down[s_max_keyboard_keys];
		pxl::u64 timestamp[s_max_keyboard_keys];
	};

	KeyboardState s_keyboardState;
}

void pxl::keyboard::onKeyDown(Key key)
{
	int i = (int)key;
	if (i > 0 && i < s_max_keyboard_keys)
	{
		s_keyboardState.down[i] = true;
		s_keyboardState.timestamp[i] = pxl::time::ticks;
	}
}

void pxl::keyboard::onKeyUp(Key key)
{
	int i = (int)key;
	if (i > 0 && i < s_max_keyboard_keys)
	{
		s_keyboardState.down[i] = false;
		s_keyboardState.timestamp[i] = pxl::time::ticks;
	}
}

bool pxl::keyboard::down(Key key)
{
	int i = (int)key;
	return i > 0 && i < s_max_keyboard_keys&& s_keyboardState.down[i];
}
