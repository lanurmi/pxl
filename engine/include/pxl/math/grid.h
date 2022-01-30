#pragma once

#include <pxl/types.h>
#include <pxl/containers/vector.h>
#include <pxl/assets/json.h>

namespace pxl
{
	class Grid
	{
	public:
		Grid(int width, int height, i16 initialValue);
		Grid();
		Grid(Grid&& other) noexcept;
		Grid(const Grid& other);
		Grid& operator=(Grid&& other) noexcept;
		Grid& operator=(const Grid& other);
		void resize(int w, int h, i16 value);
		void set(int x, int y, i16 value);
		void set(int index, i16 value);
		void set(int tox, int toy, const Grid& otherGrid);
		void setAll(i16 value);
		void flipX();
		i16 get(int x, int y) const;
		i16 get(int index) const;
		int width() const;
		int height() const;
		void floodFill(int x, int y, pxl::i16 fromValue, pxl::i16 toValue);


		PXL_JSON_SERIALIZE(Grid, _width, _height, _data);
	private:
		int _width;
		int _height;
		Vector<i16> _data;
	};
}
