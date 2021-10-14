#pragma once

#include <pxl/types.h>
#include <pxl/containers/string.h>
#include <pxl/config.h>
#include <pxl/math/vec2.h>
#include <pxl/backends/keyboard.h>
#include <functional>

namespace pxl
{
	namespace platform
	{
		void awake(const pxl::Config& config);
		void destroy();
		bool update();
		void setTitle(const String& title);
		void present();
		void sleep(int ms);
		void vsync(bool vsync);
		u64 ticks();
		Vec2 drawSize();
		Vec2 size();
		Vec2 position();
		//Gamepad
		void rumble(int index, float time, float strength);
		void inputUpdate();
		//Paths
		String applicationPath();
		String userPath();
		// Opengl
		void* glCreateContext();
		void glBindContext(void* context);
		void glDestroyContext(void* context);
		void* glGetFunc(const String& name);
		//other
		int cpuCount();

	}
}