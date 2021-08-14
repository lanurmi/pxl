#include <pxl/log.h>
#include <pxl/types.h>
#include <iostream>

using namespace pxl;

void log::message(const String& message)
{
	const String msg = "[Message] " + message;
	std::cout << msg << std::endl;
}

void log::error(const String& error)
{
	const String msg = "[Error] " + error;
	std::cout << msg << std::endl;
}
