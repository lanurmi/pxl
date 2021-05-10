#include <pxl/types.h>

using namespace pxl;

Grid::Grid(int width, int height, i16 initialValue) : _width(width), _height(height)
{
	_data = new i16[width * height];
	setAll(initialValue);
}

Grid::Grid() : _width(0), _height(0), _data(nullptr)
{

}

Grid::~Grid()
{
	delete [] _data;
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