#include <pxl/log.h>
#include <pxl/types.h>
#include <pxl/containers/string.h>
#include <iostream>

using namespace pxl;

namespace
{
	class CoutLogger : public pxl::log::ILogger {
	public:
		void message(const String &message)
		{
			std::cout << message << std::endl;
		}
		void error(const String &message)
		{
			std::cout << message << std::endl;
		}
	};

	pxl::log::ILoggerRef logger = std::make_shared<CoutLogger>();
}

void log::set(pxl::log::ILoggerRef loggerRef)
{
	logger = loggerRef;
}

void log::message(const String& message)
{
	const String msg = "[Message] " + message;
	logger->message(msg);
}

void log::error(const String& error)
{
	const String msg = "[Error] " + error;
	logger->error(msg);
}
