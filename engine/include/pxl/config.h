#pragma once
#include <pxl/types.h>
#include <pxl/containers/string.h>
#include <pxl/utils/action.h>

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
		Action<> awake;
		Action<> update;
		Action<> draw;
		Action<> destroy;
	};
}

