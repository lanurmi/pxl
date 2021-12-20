#pragma once
#include <pxl/types.h>
#include <pxl/containers/string.h>
#include <functional>

namespace pxl
{
	namespace log
	{
		class ILogger {
		public:
			virtual void message(const String &message) = 0;
			virtual void error(const String &message) = 0;
		};
		using ILoggerRef = std::shared_ptr<ILogger>;

		void set(ILoggerRef logger);
		void message(const String& message);
		void error(const String& error);
	};
}

