#pragma once
#include <unordered_map>
#include <unordered_set>

namespace pxl
{
	template<class T, class E>
	using Map = std::unordered_map<T, E>;

	template<class T>
	using Set = std::unordered_set<T>;
}