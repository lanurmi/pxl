#include <pxl/math/grid.h>
#include <queue>

using namespace pxl;

Grid::Grid(int width, int height, i16 initialValue) : _width(width), _height(height)
{
	_data.resize(width * height);
	setAll(initialValue);
}

Grid::Grid() : _width(0), _height(0)
{

}

Grid::Grid(Grid&& other) noexcept
{
	_width = other._width;
	_height = other._height;
	_data = std::move(other._data);
}

Grid::Grid(const Grid& other)
{
	_width = other._width;
	_height = other._height;
	_data = other._data;
}

Grid& Grid::operator=(Grid&& other) noexcept
{
	_width = other._width;
	_height = other._height;
	other._width = 0;
	other._height = 0;
	_data = std::move(other._data);
	return *this;
}

Grid& Grid::operator=(const Grid& other)
{
	Grid o(other);
	std::swap(o, *this);
	return *this;
}

void Grid::resize(int w, int h, i16 value)
{
	_data.resize(w * h);
	_width = w;
	_height = h;
	setAll(value);
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

void  Grid::set(int tox, int toy, const Grid& otherGrid)
{
	int dsty = toy;

	for (int j = 0; j < otherGrid.height(); j++)
	{
		int dstx = tox;
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

void Grid::flipX()
{
	pxl::Vector<pxl::i16> n(_data.size());
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			auto srcx = i;
			auto dstx = _width - 1 - i;

			auto srcIdx = j * _width + srcx;
			auto dstIdx = j * _width + dstx;

			n[dstIdx] = get(srcIdx);
		}
	}

	std::swap(n, _data);
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

void Grid::floodFill(int x, int y, pxl::i16 fromValue, pxl::i16 toValue)
{
	assert(fromValue != toValue);

	struct Point {
		Point(int x, int y) : x(x), y(y) {}
		Point() : x(0), y(0) {}
		int x;
		int y;
	};
	std::queue<Point> points;

	points.push(Point(x, y));
	while (!points.empty())
	{
		auto p = points.front();
		points.pop();

		if (get(p.x, p.y) == fromValue)
		{
			set(p.x, p.y, toValue);

			points.push(Point(p.x + 1, p.y));
			points.push(Point(p.x - 1, p.y));
			points.push(Point(p.x, p.y + 1));
			points.push(Point(p.x, p.y - 1));
		}
	}
}
