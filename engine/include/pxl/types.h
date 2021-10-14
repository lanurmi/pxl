#pragma once

#include <pxl/containers/vector.h>
#include <cstdint>
#include <functional>
#include <memory>


namespace pxl
{
	using Func = std::function<void()>;


	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
}