#include <pxl/backends/Gamepad.h>
#include <pxl/engine.h>
#include <pxl/time.h>

class Gamepadtate
{
public:
	Gamepadtate() : name(""), connected(false), gamepad(true), button_count(0), axis_count(0), vendor(0), product(0), version(0) {
		for (int i = 0; i < pxl::s_max_gamepad_buttons; i++)
		{
			pressed[i] = false;
			released[i] = false;
			down[i] = false;
			button_timestamp[i] = 0;
		}
		for (int i = 0; i < pxl::s_max_gamepad_axis; i++)
		{
			axis_timestamp[i] = 0;
			axis[i] = 0.0f;
		}
	}
	pxl::string name;
	bool connected;
	bool gamepad;
	int button_count;
	int axis_count;
	bool pressed[pxl::s_max_gamepad_buttons];
	bool down[pxl::s_max_gamepad_buttons];
	bool released[pxl::s_max_gamepad_buttons];
	float axis[pxl::s_max_gamepad_axis];
	pxl::u64 button_timestamp[pxl::s_max_gamepad_buttons];
	pxl::u64 axis_timestamp[pxl::s_max_gamepad_axis];
	pxl::u16 vendor;
	pxl::u16 product;
	pxl::u16 version;
};

static Gamepadtate s_Gamepadtate[pxl::s_max_Gamepad];
static Gamepadtate s_nextGamepadtate[pxl::s_max_Gamepad];

//
bool pxl::Gamepad::pressed(int index, Button button)
{
	int b = (int)button;
	if (index < pxl::s_max_Gamepad && b >= 0 && b < pxl::s_max_gamepad_buttons)
	{
		return s_Gamepadtate[index].pressed[b];
	}
	return false;
}
bool pxl::Gamepad::down(int index, Button button)
{
	int b = (int)button;
	if (index < pxl::s_max_Gamepad && b >= 0 && b < pxl::s_max_gamepad_buttons)
	{
		return s_Gamepadtate[index].down[b];
	}
	return false;
}

bool pxl::Gamepad::released(int index, Button button)
{
	int b = (int)button;
	if (index < pxl::s_max_Gamepad && b >= 0 && b < pxl::s_max_gamepad_buttons)
	{
		return s_Gamepadtate[index].released[b];
	}
	return false;
}

void pxl::Gamepad::rumble(int index, float time, float strength)
{
	pxl::platform().rumble(index, time, strength);
}

float pxl::Gamepad::axis(int index, Axis axis)
{
	int a = (int)axis;
	if (index < pxl::s_max_Gamepad && a >= 0 && a < pxl::s_max_gamepad_axis)
	{
		return s_Gamepadtate[index].axis[a];
	}
	return false;
}

void pxl::Gamepad::onConnect(int index, const string& name, u16 vendor, u16 product, u16 version)
{
	if (index < 0 || index >= s_max_Gamepad) return;

	s_Gamepadtate[index] = Gamepadtate();
	auto& state = s_nextGamepadtate[index];
	state.name = name;
	state.vendor = vendor;
	state.product = product;
	state.version = version;
	state.connected = true;
	state.button_count = s_max_gamepad_buttons;
	state.axis_count = s_max_gamepad_axis;
}

void pxl::Gamepad::onDisconnect(int index)
{
	if (index < 0 || index >= s_max_Gamepad) return;

	s_nextGamepadtate[index] = Gamepadtate();
}


void pxl::Gamepad::onButtonDown(int index, int button)
{
	if (index < pxl::s_max_Gamepad && button < pxl::s_max_gamepad_buttons)
	{
		auto& Gamepadtate = s_nextGamepadtate[index];
		if (Gamepadtate.connected && button < Gamepadtate.button_count)
		{
			Gamepadtate.down[button] = true;
			Gamepadtate.pressed[button] = true;
			Gamepadtate.button_timestamp[button] = pxl::time::ticks;
		}
	}
}

void pxl::Gamepad::onButtonUp(int index, int button)
{
	if (index < pxl::s_max_Gamepad && button < pxl::s_max_gamepad_buttons)
	{
		auto& Gamepadtate = s_nextGamepadtate[index];
		if (Gamepadtate.connected && button < Gamepadtate.button_count)
		{
			Gamepadtate.down[button] = false;
			Gamepadtate.released[button] = true;
		}
	}
}

void  pxl::Gamepad::onAxis(int index, int axis, float value)
{
	if (index < pxl::s_max_Gamepad && axis < pxl::s_max_gamepad_axis)
	{
		auto& gamepad = s_nextGamepadtate[index];
		if (gamepad.connected && axis < gamepad.axis_count)
		{
			gamepad.axis[axis] = value;
			gamepad.axis_timestamp[axis] = pxl::time::ticks;
		}
	}
}

void pxl::Gamepad::update()
{
	for (int i = 0; i < s_max_Gamepad; i++)
	{
		s_Gamepadtate[i] = s_nextGamepadtate[i];
		auto& Gamepadtate = s_nextGamepadtate[i];
		if (Gamepadtate.connected)
		{
			for (int b = 0; b < s_max_gamepad_buttons; b++)
			{
				Gamepadtate.pressed[b] = false;
				Gamepadtate.released[b] = false;
			}
		}
	}
}

