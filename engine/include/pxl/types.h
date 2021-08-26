#pragma once
#include <pxl/containers/string.h>
#include <pxl/containers/vector.h>
#include <pxl/containers/map.h>
#include <pxl/containers/array.h>

#include <cstdint>
#include <functional>
#include <memory>

#include <stack>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <assert.h>
#include <algorithm>

namespace pxl
{
	using Func = std::function<void()>;


	template<class T>
	using Stack = std::stack<T>;

	template<class T>
	using Deque = std::deque<T>;

	template<class T>
	using Set = std::set<T>;

	template<class T>
	using Unordered_set = std::unordered_set<T>;

	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	class Grid
	{
	public:
		Grid(int width, int height, i16 initialValue);
		Grid();
		Grid(Grid &&other) noexcept;
		Grid& operator=(Grid &&other) noexcept;
		void set(int x, int y, i16 value);
		void set(int index, i16 value);
		void set(int fromx, int fromy, const Grid& otherGrid);
		void setAll(i16 value);
		i16 get(int x, int y) const;
		i16 get(int index) const;
		int width() const;
		int height() const;
	private:
		int _width;
		int _height;
		Vector<i16> _data;
	};

}