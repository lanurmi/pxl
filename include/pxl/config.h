#pragma once
#include <pxl/types.h>

namespace pxl
{
	struct Config
	{
		Config();
		string name;
		string title;
		int width;
		int height;
		int target_framerate;
		bool fixed_update;
		bool vertical_sync;
		Func onBegin;
		Func onEnd;
		Func onUpdate;
		Func onDraw;
		std::function<void(const string&)> onLog;
	};
}