#include <pxl/log.h>
#include <iostream>

using namespace pxl;

void Log::message(const string& message)
{
	const string msg = "[Message] " + message;
	if (onLog != nullptr)
	{
		onLog(msg);
	}
	else
	{
		std::cout << msg << std::endl;
	}
}

void Log::error(const string& error)
{
	const string msg = "[Error] " + error;
	if (onLog != nullptr)
	{
		onLog(msg);
	}
	else
	{
		std::cout << msg << std::endl;
	}
}
