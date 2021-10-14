#pragma once
#include <pxl/types.h>

namespace pxl
{
	class PlatformBackend;

	constexpr int s_max_gamepad_buttons = 64;
	constexpr int s_max_gamepad_axis = 16;
	constexpr int s_max_Gamepad = 16;

	enum class Button 	{
		Invalid = -1,
		A = 0,
		B = 1,
		X = 2,
		Y = 3,
		Back = 4,
		Select = 5,
		Start = 6,
		LeftStick = 7,
		RightStick = 8,
		LeftShoulder = 9,
		RightShoulder = 10,
		Up = 11,
		Down = 12,
		Left = 13,
		Right = 14
	};

	enum class Axis 	{
		Invalid = -1,
		LeftX = 0,
		LeftY = 1,
		RightX = 2,
		RightY = 3,
		LeftTrigger = 4,
		RightTrigger = 5
	};

	namespace gamepad
	{
		bool pressed(int index, Button key);
		bool down(int index, Button key);
		bool released(int index, Button key);
		void rumble(int index, float time, float strength);
		float axis(int index, Axis axis);

		void onConnect(int index, const String& name, u16 vendor, u16 product, u16 version); // called by platform
		void onDisconnect(int index); // called by platform
		void onButtonDown(int index, int button); // called by platform
		void onButtonUp(int index, int button); // called by platform
		void onAxis(int index, int axis, float value); // called by platform
	};
}
