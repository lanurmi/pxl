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
		pxl::String inputText;
	};

	KeyboardState s_keyboardState;
}

void pxl::keyboard::resetText() {
	s_keyboardState.inputText.clear();
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

const pxl::String &pxl::keyboard::text() {
	return s_keyboardState.inputText;
}

void pxl::keyboard::onText(const char* text) {
	s_keyboardState.inputText.append(text);
}

bool pxl::keyboard::down(Key key)
{
	int i = (int)key;
	return i > 0 && i < s_max_keyboard_keys&& s_keyboardState.down[i];
}
