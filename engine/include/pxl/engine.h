#pragma once

#include <pxl/types.h>
#include <functional>
#include <pxl/config.h>
#include <pxl/backends/platform_backend.h>
#include <pxl/backends/graphics_backend.h>
#include <pxl/backends/gamepad.h>
#include <pxl/backends/keyboard.h>
#include <pxl/backends/mouse_backend.h>
#include <pxl/utils/scene_manager.h>
#include <pxl/utils/input_binding.h>
#include <pxl/content.h>
#include <pxl/log.h>

namespace pxl
{
	class Engine
	{
	public:
		static Engine& instance();
		void begin(const Config& config);
		GraphicsBackend& graphics();
		PlatformBackend& platform();
		Gamepad& gamepad();
		KeyboardBackend& keyboard();
		Bindings& bindings();
		Content& content();
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
		PlatformBackend _platform;
		GraphicsBackend _graphics;
		Gamepad _gamepad;
		KeyboardBackend _keyboard;
		Content _content;
		Bindings _bindings;
		Log _log;
		MouseBackend _mouse;
		SceneManager _scene_manager;
	};

	Engine& engine();
	Bindings& bindings();
	SceneManager& sceneManager();
	Log& log();
	Content& content();

	GraphicsBackend& graphics();
	PlatformBackend& platform();
	MouseBackend& mouse();
	Gamepad& gamepad();
	KeyboardBackend& keyboard();
}