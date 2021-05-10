#pragma once
#include <pxl/types.h>
#include <pxl/input/gamepad_buttons.h>

namespace pxl
{
	class PlatformBackend;

	constexpr int s_max_gamepad_buttons = 64;
	constexpr int s_max_gamepad_axis = 16;
	constexpr int s_max_gamepads = 16;

	class GamepadsBackend
	{
	public:
		bool pressed(int index, Button key);
		bool down(int index, Button key);
		bool released(int index, Button key);
		void rumble(int index, float time, float strength);
		float axis(int index, Axis axis);
	private:
		void update(); //called by engine
		void onConnect(int index, const string& name, u16 vendor, u16 product, u16 version);
		void onDisconnect(int index);
		void onButtonDown(int index, int button);
		void onButtonUp(int index, int button);
		void onAxis(int index, int axis, float value);
		friend class pxl::PlatformBackend;
	};
}
