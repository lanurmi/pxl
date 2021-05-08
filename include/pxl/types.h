#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <memory>
#include <vector>
#include <stack>
#include <map>
#include <set>

namespace pxl
{
	using Func = std::function<void()>;
	using string = std::string;

	template<class T>
	using vector = std::vector<T>;

	template<class T>
	using stack = std::stack<T>;

	template<class T>
	using deque = std::deque<T>;

	template<class T, class E>
	using map = std::map<T,E>;

	template<class T>
	using set = std::set<T>;

	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	template<class T>
	string to_string(T v)
	{
		return std::to_string(v);
	}

	template<class T>
	void remove_all(vector<T>& from, T value)
	{
		from.erase(std::remove(from.begin(), from.end(), value), from.end());
	}
}