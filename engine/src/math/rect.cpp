#include <pxl/math/Rect.h>
#include <pxl/math/mat3x2.h>

pxl::Rect::Rect()
{
	x = y = width = height = 0;
};

pxl::Rect::Rect(float rx, float ry, float rw, float rh)
{
	x = rx;
	y = ry;
	width = rw;
	height = rh;
}

pxl::Rect::Rect(const Vec2& pos, const Vec2& size)
{
	x = pos.x;
	y = pos.y;
	width = size.x;
	height = size.y;
}

pxl::Rect::Rect(const Vec2& tl, const Vec2& tr, const Vec2& br, const Vec2& bl) {
	x = tl.x;
	y = tl.y;
	width = (tr.x - tl.x) + 1;
	height = (br.y - tr.y) + 1;
}

bool pxl::Rect::operator==(const Rect& rhs) const { return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height; }
bool pxl::Rect::operator!=(const Rect& rhs) const { return x != rhs.x || y != rhs.y || width != rhs.width || height != rhs.height; }
pxl::Rect pxl::Rect::operator+(const Vec2& rhs) const { return pxl::Rect(x + rhs.x, y + rhs.y, width, height); }
pxl::Rect pxl::Rect::operator-(const Vec2& rhs) const { return pxl::Rect(x - rhs.x, y - rhs.y, width, height); }
pxl::Rect& pxl::Rect::operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
pxl::Rect& pxl::Rect::operator-=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }

pxl::Rect pxl::Rect::scale(float s)
{
	x = (x * s);
	y = (y * s);
	width = (width * s);
	height = (height * s);
	return *this;
}

pxl::Rect pxl::Rect::scale(float sx, float sy)
{
	x = (x * sx);
	y = (y * sy);
	width = (width * sx);
	height = (height * sy);
	return *this;
}

float pxl::Rect::left() const
{
	return x;
}

float pxl::Rect::right() const
{
	return x + width;
}

float pxl::Rect::top() const
{
	return y;
}

float pxl::Rect::bottom() const
{
	return y + height;
}

pxl::Vec2 pxl::Rect::center() const
{
	return Vec2(x + width / 2, y + height / 2);
}

float pxl::Rect::centerX() const
{
	return x + width / 2;
}

float pxl::Rect::centerY() const
{
	return y + height / 2;
}

pxl::Vec2 pxl::Rect::topLeft() const
{
	return Vec2(x, y);
}

pxl::Vec2 pxl::Rect::topRight() const
{
	return Vec2(x + width, y);
}

pxl::Vec2 pxl::Rect::bottomRight() const
{
	return Vec2(x + width, y + height);
}

pxl::Vec2 pxl::Rect::bottomLeft() const
{
	return Vec2(x, y + height);
}

pxl::Vec2 pxl::Rect::centerLeft() const
{
	return Vec2(x, y + height / 2);
}

pxl::Vec2 pxl::Rect::centerRight() const
{
	return Vec2(x + width, y + height / 2);
}

pxl::Vec2 pxl::Rect::middleTop() const
{
	return Vec2(x + width / 2, y);
}

pxl::Vec2 pxl::Rect::middleBottom() const
{
	return Vec2(x + width / 2, y + height);
}

pxl::Vec2 pxl::Rect::size() const
{
	return Vec2(width, height);
}

bool pxl::Rect::contains(const Vec2& pt) const
{
	return pt.x >= x && pt.x < x + width && pt.y >= y && pt.y < y + height;
}

bool pxl::Rect::overlaps(const pxl::Rect& rect) const
{
	return x + width >= rect.x && y + height >= rect.y && x < rect.x + rect.width && y < rect.y + rect.height;
}


pxl::Rect pxl::Rect::inflate(float amount) const
{
	return pxl::Rect(x - amount, y - amount, width + amount * 2, height + amount * 2);
}


pxl::Rect pxl::Rect::transform(const pxl::Rect& rect, const pxl::Mat3x2& matrix)
{
	return pxl::Rect(
		(rect.x * matrix.m11) + (rect.y * matrix.m21) + matrix.m31,
		(rect.x * matrix.m12) + (rect.y * matrix.m22) + matrix.m32,
		(rect.width * matrix.m11) + (rect.height * matrix.m21),
		(rect.width * matrix.m12) + (rect.height * matrix.m22));
}
