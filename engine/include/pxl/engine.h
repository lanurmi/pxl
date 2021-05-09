#pragma once

#include <pxl/types.h>
#include <functional>
#include <pxl/config.h>
#include <pxl/platforms/platform_backend.h>
#include <pxl/graphics/graphics_backend.h>
#include <pxl/input/gamepad_backend.h>
#include <pxl/input/keyboard_backend.h>
#include <pxl/input/mouse_backend.h>
#include <pxl/utils/scene_manager.h>
#include <pxl/log.h>

namespace pxl
{
	class Engine
	{
	public:
		static Engine& instance();
		void start(const Config& config);
		GraphicsBackend& graphics();
		PlatformBackend& platform();
		GamepadsBackend& gamepads();
		KeyboardBackend& keyboard();
		MouseBackend& mouse();
		SceneManager& sceneManager();
		Log& log();
		Vec2 drawSize() const;
		Vec2 size() const;
		string applicationPath() const;
		string userPath() const;
		void end();
	private:
		Engine();
		void inputUpdate();
		PlatformBackend _platform;
		GraphicsBackend _graphics;
		GamepadsBackend _gamepads;
		KeyboardBackend _keyboard;
		Log _log;
		MouseBackend _mouse;
		SceneManager _scene_manager;
	};

	Engine& engine();
	GraphicsBackend& graphics();
	PlatformBackend& platform();
	GamepadsBackend& gamepads();
	KeyboardBackend& keyboard();
	MouseBackend& mouse();
	SceneManager& sceneManager();
	Log& log();
}