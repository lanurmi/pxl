#pragma once
#include <pxl/types.h>
#include <pxl/input/keyboard_keys.h>

namespace pxl
{
	class PlatformBackend;

	class KeyboardBackend
	{
	public:
		//
		bool pressed(Key key);
		bool down(Key key);
		bool released(Key key);
	private:
		void update();
		void onKeyDown(Key key);
		void onKeyUp(Key key);
		friend class pxl::PlatformBackend;
	};
}