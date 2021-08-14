#pragma once
#include <pxl/types.h>
namespace pxl
{
	struct Config
	{
		Config();
		String name;
		String title;
		int width;
		int height;
		int target_framerate;
		bool fixed_update;
		bool vertical_sync;
		Func awake;
	};
}