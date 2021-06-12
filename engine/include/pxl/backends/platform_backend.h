#pragma once

#include <pxl/types.h>
#include <pxl/config.h>
#include <pxl/math/vec2.h>
#include <pxl/backends/keyboard.h>
#include <functional>

namespace pxl
{
	class PlatformBackend
	{
	public:
		PlatformBackend();
		~PlatformBackend();
		void init(const pxl::Config& config);
		void shutdown();
		void update();
		void setTitle(const string& title);
		void present();
		void sleep(int ms);
		void vsync(bool vsync);
		u64 ticks();
		Vec2 drawSize() const;
		Vec2 size() const;
		Vec2 position() const;
		//Gamepad
		void rumble(int index, float time, float strength);
		void inputUpdate();
		//Paths
		string applicationPath() const;
		string userPath() const;
		// Opengl
		void* glCreateContext();
		void glBindContext(void* context);
		void glDestroyContext(void* context);
		void* glGetFunc(const string& name);
		Func onEnd;
	};
}