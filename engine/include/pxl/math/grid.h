#pragma once

#include <pxl/types.h>
#include <pxl/containers/vector.h>

namespace pxl
{
	class Grid
	{
	public:
		Grid(int width, int height, i16 initialValue);
		Grid();
		Grid(Grid&& other) noexcept;
		Grid& operator=(Grid&& other) noexcept;
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