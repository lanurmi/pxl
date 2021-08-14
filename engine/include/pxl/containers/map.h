#pragma once
#include <unordered_map>

namespace pxl
{
	template<class T, class E>
	using Map = std::unordered_map<T, E>;
}