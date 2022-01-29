#include <pxl/engine.h>
#include <pxl/types.h>
#include <pxl/containers/string.h>
#include <pxl/time.h>


namespace
{
	bool s_end = false;
	bool updateInternal()
	{
		bool rtn = pxl::platform::update();
		pxl::bindings::update();
		return rtn;
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
}

void pxl::begin(const pxl::Config& config)
{
	s_end = false;
	pxl::platform::awake(config);
	
	if (pxl::graphics::api() == pxl::graphics::Api::OpenGL)
	{
		auto ctx = pxl::platform::glCreateContext();
	}

	pxl::graphics::awake();
	pxl::platform::vsync(config.vertical_sync);
	pxl::audio::awake();

	u64 time_last = pxl::platform::ticks();
	u64 time_accumulator = 0;
	u64 fps_update = time_last + time::ticks_per_second;
	
	Fps fps;
//#ifdef PXLDEBUG
	fps.onFps = [&config](int fps)
	{
		pxl::platform::setTitle(pxl::String::format("%s - %d", config.title.c_str(), fps));
	};
//#endif

	config.awake.invoke();

	while (!s_end)
	{

		auto time_curr = pxl::platform::ticks();
		pxl::time::ticks = time_curr;
		auto time_diff = time_curr - time_last;
		time_last = time_curr;
		pxl::time::true_delta = (double)time_diff / pxl::time::ticks_per_second;

		if (config.fixed_update)
		{
			auto time_target = (u64)((1.0 / config.target_framerate) * time::ticks_per_second);
			time_accumulator += time_diff;
			while (time_accumulator < time_target)
			{
				int milliseconds = (int)(time_target - time_accumulator) / (time::ticks_per_second / 1000);
				pxl::platform::sleep(milliseconds);
				time_curr = pxl::platform::ticks();
				time_diff = time_curr - time_last;
				time_last = time_curr;
				time_accumulator += time_diff;
			}
			u64 time_maximum = 5 * time_target;
			if (time_accumulator > time_maximum)
			{
				time_accumulator = time_maximum;
			}
			while (time_accumulator >= time_target)
			{
				time_accumulator -= time_target;

				time::delta = 1.0f / config.target_framerate;
				time::unscaled_delta = time::delta;
				time::delta *= pxl::time::scale;
				pxl::time::seconds += time::delta;

				if (!updateInternal())
				{
					s_end = true;
					continue;
				}

				config.update.invoke();
			}
		}
		else
		{
			pxl::time::delta = (double)time_diff / pxl::time::ticks_per_second;
			time::unscaled_delta = time::delta;
			pxl::time::delta *= pxl::time::scale;
			pxl::time::seconds += time::delta;

			if (!updateInternal())
			{
				s_end = true;
				continue;
			}

			config.update.invoke();
		}


		config.draw.invoke();
		pxl::platform::present();

		fps.update();
	}
	config.destroy.invoke();

	pxl::audio::destroy();
	pxl::graphics::destroy();
	pxl::platform::destroy();
}

void pxl::end()
{
	s_end = true;
}
