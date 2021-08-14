#pragma once
#include <pxl/types.h>
#include <functional>

namespace pxl
{
	namespace log
	{
		void message(const String& message);
		void error(const String& error);
	};
}