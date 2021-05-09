#pragma once
#include <pxl/types.h>
#include <functional>

namespace pxl
{
	class Log
	{
	public:
		void message(const string& message);
		void error(const string& error);
		std::function<void(const string&)> onLog;
	};
}