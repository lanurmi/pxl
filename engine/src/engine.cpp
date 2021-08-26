#include <pxl/engine.h>
#include <pxl/types.h>
#include <pxl/time.h>



namespace
{
	bool s_end = false;
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

void pxl::begin(const pxl::Config& config)
{
	s_end = false;
	pxl::platform::init(config);
	
	pxl::graphics::bind();
	pxl::platform::vsync(config.vertical_sync);

	u64 time_last = pxl::platform::ticks();
	u64 time_accumulator = 0;
	u64 fps_update = time_last + time::ticks_per_second;
	
	Fps fps;
//#ifdef PXLDEBUG
	fps.onFps = [](int fps)
	{

		pxl::platform::setTitle(String::fromInt(fps));

	};
//#endif

	if (config.awake)
	{
		config.awake();
	}
	while (!s_end)
	{

		auto time_curr = pxl::platform::ticks();
		pxl::time::ticks = time_curr;
		auto time_diff = time_curr - time_last;
		time_last = time_curr;
		pxl::time::true_delta = (double)time_diff / pxl::time::ticks_per_second;

		fps.update();

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
			time::delta = 1.0f / config.target_framerate;
			time::unscaled_delta = time::delta;
			time::delta *= pxl::time::scale;
			while (time_accumulator >= time_target)
			{
				time_accumulator -= time_target;
				if (!pxl::platform::update())
				{
					s_end = true;
				}
				//Input things
				pxl::mouse::update();
				pxl::keyboard::update();
				pxl::gamepad::update();

				pxl::bindings::update();
				pxl::scenes::update();
			}
		}
		else
		{
			pxl::time::delta = (double)time_diff / pxl::time::ticks_per_second;
			time::unscaled_delta = time::delta;
			pxl::time::delta *= pxl::time::scale;
			if (!pxl::platform::update())
			{
				s_end = true;
				continue;
			}

			//Input things
			pxl::mouse::update();
			pxl::keyboard::update();
			pxl::gamepad::update();

			pxl::bindings::update();
			pxl::scenes::update();
		}

		pxl::scenes::draw();
		pxl::platform::present();
	}

	pxl::scenes::end();
	pxl::graphics::unbind();
	pxl::platform::shutdown();
}

void pxl::end()
{
	s_end = true;
}
