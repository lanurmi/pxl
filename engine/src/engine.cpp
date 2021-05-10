#include <pxl/engine.h>
#include <pxl/types.h>
#include <pxl/time.h>

static bool s_end = false;

pxl::Engine::Engine()
{
}

pxl::Engine& pxl::Engine::instance()
{
	static Engine engine;
	return engine;
}

pxl::GraphicsBackend& pxl::Engine::graphics()
{
	return _graphics;
}

pxl::PlatformBackend& pxl::Engine::platform()
{
	return _platform;
}

pxl::GamepadsBackend& pxl::Engine::gamepads()
{
	return _gamepads;
}

pxl::KeyboardBackend& pxl::Engine::keyboard()
{
	return _keyboard;
}

pxl::Bindings& pxl::Engine::bindings()
{
	return _bindings;
}

pxl::MouseBackend& pxl::Engine::mouse()
{
	return _mouse;
}

pxl::SceneManager& pxl::Engine::sceneManager()
{
	return _scene_manager;
}

pxl::Log& pxl::Engine::log()
{
	return _log;
}

pxl::Vec2 pxl::Engine::drawSize() const
{
	return _platform.drawSize();
}

pxl::Vec2 pxl::Engine::size() const
{
	return _platform.size();
}

pxl::string pxl::Engine::applicationPath() const
{
	return _platform.applicationPath();
}

pxl::string pxl::Engine::userPath() const
{
	return _platform.userPath();
}

class Fps
{
public:
	Fps() : _fps_update(0), _frame_counter(0)
	{
		_fps_update = pxl::time::ticks + pxl::time::ticks_per_second;
	}
	void update()
	{
		_frame_counter++;
		if (pxl::time::ticks >= _fps_update)
		{
			if (onFps != nullptr)
			{
				onFps(_frame_counter);
			}
			_frame_counter = 0;
			_fps_update = pxl::time::ticks + pxl::time::ticks_per_second;
		}
	}
	std::function<void(int)> onFps;
private:
	int _frame_counter;
	pxl::u64 _fps_update;
};

void pxl::Engine::begin(const pxl::Config& config)
{
	s_end = false;
	_platform.init(config);
	_platform.onEnd = []()
	{
		s_end = true;
	};
	
	_graphics.bind(_platform);
	_platform.vsync(config.vertical_sync);

	u64 time_last = _platform.ticks();
	u64 time_accumulator = 0;
	u64 fps_update = time_last + time::ticks_per_second;
	
	Fps fps;
//#ifdef PXLDEBUG
	fps.onFps = [this](int fps)
	{

		_platform.setTitle(to_string(fps));

	};
//#endif

	while (!s_end)
	{

		auto time_curr = _platform.ticks();
		pxl::time::ticks = time_curr;
		auto time_diff = time_curr - time_last;
		time_last = time_curr;
		pxl::time::true_delta = (double)time_diff / pxl::time::ticks_per_second;

		fps.update();
		_platform.update();

		if (config.fixed_update)
		{
			auto time_target = (u64)((1.0 / config.target_framerate) * time::ticks_per_second);
			time_accumulator += time_diff;
			while (time_accumulator < time_target)
			{
				int milliseconds = (int)(time_target - time_accumulator) / (time::ticks_per_second / 1000);
				_platform.sleep(milliseconds);
				time_curr = _platform.ticks();
				time_diff = time_curr - time_last;
				time_last = time_curr;
				time_accumulator += time_diff;
			}
			u64 time_maximum = 5 * time_target;
			if (time_accumulator > time_maximum)
			{
				time_accumulator = time_maximum;
			}
			time::delta = 1.0f / config.target_framerate;
			while (time_accumulator >= time_target)
			{
				time_accumulator -= time_target;
				_platform.inputUpdate();
				_bindings.update();
				_scene_manager.update();
			}
		}
		else
		{
			pxl::time::delta = (double)time_diff / pxl::time::ticks_per_second;
			_platform.inputUpdate();
			_bindings.update();
			_scene_manager.update();
		}

		_scene_manager.draw();
		_platform.present();
	}

	_scene_manager.end();
	_graphics.unbind(_platform);
	_platform.shutdown();
}

void pxl::Engine::end()
{
	s_end = true;
}

pxl::Engine& pxl::engine()
{
	return pxl::Engine::instance();
}

pxl::GraphicsBackend& pxl::graphics()
{
	return pxl::Engine::instance().graphics();
}

pxl::PlatformBackend& pxl::platform()
{
	return pxl::Engine::instance().platform();
}

pxl::GamepadsBackend& pxl::gamepads()
{
	return pxl::Engine::instance().gamepads();
}

pxl::KeyboardBackend& pxl::keyboard()
{
	return pxl::Engine::instance().keyboard();
}

pxl::Bindings& pxl::bindings()
{
	return pxl::Engine::instance().bindings();
}

pxl::MouseBackend& pxl::mouse()
{
	return pxl::Engine::instance().mouse();
}

pxl::Log& pxl::log()
{
	return pxl::Engine::instance().log();
}

pxl::SceneManager& pxl::sceneManager()
{
	return pxl::Engine::instance().sceneManager();
}