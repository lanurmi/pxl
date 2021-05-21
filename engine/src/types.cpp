#include <pxl/types.h>

using namespace pxl;

Grid::Grid(int width, int height, i16 initialValue) : _width(width), _height(height)
{
	_data.resize(width * height);
	setAll(initialValue);
}

Grid::Grid() : _width(0), _height(0)
{

}

Grid::~Grid()
{
}

void Grid::set(int x, int y, i16 value)
{
	auto idx = y * _width + x;
	set(idx, value);
}

void Grid::set(int index, i16 value)
{
	_data[index] = value;
}

void  Grid::set(int fromx, int fromy, const Grid& otherGrid)
{
	int dsty = fromy;

	for (int j = 0; j < otherGrid.height(); j++)
	{
		int dstx = fromx;
		for (int i = 0; i < otherGrid.width(); i++)
		{
			set(dstx, dsty, otherGrid.get(i, j));
			dstx++;
		}
		dsty++;
	}
}

void Grid::setAll(i16 value)
{
	for (int i = 0; i < _width * _height; i++)
	{
		_data[i] = value;
	}
}

i16 Grid::get(int x, int y) const
{
	auto idx = y * _width + x;
	return get(idx);
}

i16 Grid::get(int index) const
{
	return _data[index];
}

int Grid::width() const
{
	return _width;
}

int Grid::height() const
{
	return _height;
}