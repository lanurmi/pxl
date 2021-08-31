#include <pxl/time.h>
#include <chrono>

using namespace pxl;

const u64 time::ticks_per_second = 1000000;
float time::seconds = 0.0f;
float time::delta = 0.0f;
float time::unscaled_delta = 0.0f;
float time::scale = 1.0f;
float time::true_delta = 0.0f;
u64 time::ticks = 0;


bool time::interval(float interval)
{
	auto l = static_cast<long>( (time::seconds - time::delta) / interval);
	float n = static_cast<long>(time::seconds / interval);
	return l < n;
}


time::Timer::Timer()
{
	reset();
}

void time::Timer::reset()
{
	start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

u64 time::Timer::ms()
{
	auto u = us();
	return u / 1000UL;
}

u64 time::Timer::us()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - start;
}
