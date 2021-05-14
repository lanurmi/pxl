#include <pxl/input/gamepad_backend.h>
#include <pxl/engine.h>
#include <pxl/time.h>

class GamepadState
{
public:
	GamepadState() : name(""), connected(false), gamepad(true), button_count(0), axis_count(0), vendor(0), product(0), version(0) {
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

static GamepadState s_gamepadState[pxl::s_max_gamepads];
static GamepadState s_nextGamepadState[pxl::s_max_gamepads];

//
bool pxl::GamepadsBackend::pressed(int index, Button button)
{
	int b = (int)button;
	if (index < pxl::s_max_gamepads && b >= 0 && b < pxl::s_max_gamepad_buttons)
	{
		return s_gamepadState[index].pressed[b];
	}
	return false;
}
bool pxl::GamepadsBackend::down(int index, Button button)
{
	int b = (int)button;
	if (index < pxl::s_max_gamepads && b >= 0 && b < pxl::s_max_gamepad_buttons)
	{
		return s_gamepadState[index].down[b];
	}
	return false;
}

bool pxl::GamepadsBackend::released(int index, Button button)
{
	int b = (int)button;
	if (index < pxl::s_max_gamepads && b >= 0 && b < pxl::s_max_gamepad_buttons)
	{
		return s_gamepadState[index].released[b];
	}
	return false;
}

void pxl::GamepadsBackend::rumble(int index, float time, float strength)
{
	pxl::platform().rumble(index, time, strength);
}

float pxl::GamepadsBackend::axis(int index, Axis axis)
{
	int a = (int)axis;
	if (index < pxl::s_max_gamepads && a >= 0 && a < pxl::s_max_gamepad_axis)
	{
		return s_gamepadState[index].axis[a];
	}
	return false;
}

void pxl::GamepadsBackend::onConnect(int index, const string& name, u16 vendor, u16 product, u16 version)
{
	if (index < 0 || index >= s_max_gamepads) return;

	s_gamepadState[index] = GamepadState();
	auto& state = s_nextGamepadState[index];
	state.name = name;
	state.vendor = vendor;
	state.product = product;
	state.version = version;
	state.connected = true;
	state.button_count = s_max_gamepad_buttons;
	state.axis_count = s_max_gamepad_axis;
}

void pxl::GamepadsBackend::onDisconnect(int index)
{
	if (index < 0 || index >= s_max_gamepads) return;

	s_nextGamepadState[index] = GamepadState();
}


void pxl::GamepadsBackend::onButtonDown(int index, int button)
{
	if (index < pxl::s_max_gamepads && button < pxl::s_max_gamepad_buttons)
	{
		auto& gamepadState = s_nextGamepadState[index];
		if (gamepadState.connected && button < gamepadState.button_count)
		{
			gamepadState.down[button] = true;
			gamepadState.pressed[button] = true;
			gamepadState.button_timestamp[button] = pxl::time::ticks;
		}
	}
}

void pxl::GamepadsBackend::onButtonUp(int index, int button)
{
	if (index < pxl::s_max_gamepads && button < pxl::s_max_gamepad_buttons)
	{
		auto& gamepadState = s_nextGamepadState[index];
		if (gamepadState.connected && button < gamepadState.button_count)
		{
			gamepadState.down[button] = false;
			gamepadState.released[button] = true;
		}
	}
}

void  pxl::GamepadsBackend::onAxis(int index, int axis, float value)
{
	if (index < pxl::s_max_gamepads && axis < pxl::s_max_gamepad_axis)
	{
		auto& gamepad = s_nextGamepadState[index];
		if (gamepad.connected && axis < gamepad.axis_count)
		{
			gamepad.axis[axis] = value;
			gamepad.axis_timestamp[axis] = pxl::time::ticks;
		}
	}
}

void pxl::GamepadsBackend::update()
{
	for (int i = 0; i < s_max_gamepads; i++)
	{
		s_gamepadState[i] = s_nextGamepadState[i];
		auto& gamepadState = s_nextGamepadState[i];
		if (gamepadState.connected)
		{
			for (int b = 0; b < s_max_gamepad_buttons; b++)
			{
				gamepadState.pressed[b] = false;
				gamepadState.released[b] = false;
			}
		}
	}
}

