#pragma once

#include <pxl/types.h>
#include <functional>
#include <pxl/config.h>
#include <pxl/backends/platform_backend.h>
#include <pxl/backends/graphics_backend.h>
#include <pxl/backends/gamepad.h>
#include <pxl/backends/keyboard.h>
#include <pxl/backends/mouse.h>
#include <pxl/utils/scene_manager.h>
#include <pxl/utils/input_binding.h>
#include <pxl/log.h>

namespace pxl
{
	void begin(const Config& config);
	void end();
}