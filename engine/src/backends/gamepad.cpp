#include <pxl/backends/Gamepad.h>
#include <pxl/engine.h>
#include <pxl/time.h>

namespace
{
	class Gamepadtate
	{
	public:
		Gamepadtate() : name(""), connected(false), gamepad(true), button_count(0), axis_count(0), vendor(0), product(0), version(0) {
			for (int i = 0; i < pxl::s_max_gamepad_buttons; i++)
			{
				down[i] = false;
				button_timestamp[i] = 0;
			}
			for (int i = 0; i < pxl::s_max_gamepad_axis; i++)
			{
				axis_timestamp[i] = 0;
				axis[i] = 0.0f;
			}
		}
		pxl::String name;
		bool connected;
		bool gamepad;
		int button_count;
		int axis_count;
		bool down[pxl::s_max_gamepad_buttons];
		float axis[pxl::s_max_gamepad_axis];
		pxl::u64 button_timestamp[pxl::s_max_gamepad_buttons];
		pxl::u64 axis_timestamp[pxl::s_max_gamepad_axis];
		pxl::u16 vendor;
		pxl::u16 product;
		pxl::u16 version;
	};

	Gamepadtate s_Gamepadtate[pxl::s_max_Gamepad];
}
//

bool pxl::gamepad::down(int index, Button button)
{
	int b = (int)button;
	if (index < pxl::s_max_Gamepad && b >= 0 && b < pxl::s_max_gamepad_buttons)
	{
		return s_Gamepadtate[index].down[b];
	}
	return false;
}

void pxl::gamepad::rumble(int index, float time, float strength)
{
	pxl::platform::rumble(index, time, strength);
}

float pxl::gamepad::axis(int index, Axis axis)
{
	int a = (int)axis;
	if (index < pxl::s_max_Gamepad && a >= 0 && a < pxl::s_max_gamepad_axis)
	{
		return s_Gamepadtate[index].axis[a];
	}
	return false;
}

void pxl::gamepad::onConnect(int index, const String& name, u16 vendor, u16 product, u16 version)
{
	if (index < 0 || index >= s_max_Gamepad) return;

	s_Gamepadtate[index] = Gamepadtate();
	auto& state = s_Gamepadtate[index];
	state.name = name;
	state.vendor = vendor;
	state.product = product;
	state.version = version;
	state.connected = true;
	state.button_count = s_max_gamepad_buttons;
	state.axis_count = s_max_gamepad_axis;
}

void pxl::gamepad::onDisconnect(int index)
{
	if (index < 0 || index >= s_max_Gamepad) return;

	s_Gamepadtate[index] = Gamepadtate();
}


void pxl::gamepad::onButtonDown(int index, int button)
{
	if (index < pxl::s_max_Gamepad && button < pxl::s_max_gamepad_buttons)
	{
		auto& Gamepadtate = s_Gamepadtate[index];
		if (Gamepadtate.connected && button < Gamepadtate.button_count)
		{
			Gamepadtate.down[button] = true;
			Gamepadtate.button_timestamp[button] = pxl::time::ticks;
		}
	}
}

void pxl::gamepad::onButtonUp(int index, int button)
{
	if (index < pxl::s_max_Gamepad && button < pxl::s_max_gamepad_buttons)
	{
		auto& Gamepadtate = s_Gamepadtate[index];
		if (Gamepadtate.connected && button < Gamepadtate.button_count)
		{
			Gamepadtate.down[button] = false;
		}
	}
}

void pxl::gamepad::onAxis(int index, int axis, float value)
{
	if (index < pxl::s_max_Gamepad && axis < pxl::s_max_gamepad_axis)
	{
		auto& gamepad = s_Gamepadtate[index];
		if (gamepad.connected && axis < gamepad.axis_count)
		{
			gamepad.axis[axis] = value;
			gamepad.axis_timestamp[axis] = pxl::time::ticks;
		}
	}
}
