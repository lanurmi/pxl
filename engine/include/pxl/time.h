#pragma once
#include <pxl/Types.h>

namespace pxl
{
	namespace time
	{
		extern const u64 ticks_per_second;
		extern float delta;
		extern float unscaled_delta;
		extern float scale;
		extern float true_delta;
		extern float seconds;
		extern u64 ticks;

		bool interval(float interval);
		class Timer
		{
		public:
			Timer();
			void reset();
			u64 ms();
			u64 us();
		private:
			u64 start;
		};
	}
}