#include <pxl/math/color.h>

pxl::Color::Color() : r(0), g(0), b(0), a(255)
{
}

pxl::Color::Color(u8 r, u8 g, u8 b) : r(r), g(g), b(b), a(255)
{
}

pxl::Color::Color(u8 r, u8 g, u8 b, u8 a) : r(r), g(g), b(b), a(a)
{
}

pxl::Color::Color(u32 color)
{
	r = (u8)((color & 0xFF000000) >> 24);
	g = (u8)((color & 0x00FF0000) >> 16);
	b = (u8)((color & 0x0000FF00) >> 8);
	a = (u8)((color & 0x000000FF));
}

bool pxl::Color::operator==(const Color& color) const
{
	return r == color.r && g == color.g && b == color.b && a == color.a;
}

bool pxl::Color::operator!=(const Color& color) const
{
	return r != color.r || g != color.g || b != color.b || a != color.a;
}

pxl::Color pxl::Color::operator +(const Color& rhs) const
{
	return pxl::Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
}

pxl::Color pxl::Color::operator -(const Color& rhs) const
{
	return pxl::Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
}

pxl::Color pxl::Color::operator /(const float rhs) const
{
	return pxl::Color(r / rhs, g / rhs, b / rhs, a / rhs);
}

pxl::Color pxl::Color::operator *(const float rhs) const 
{
	return pxl::Color((float)r * rhs, (float)g * rhs, (float)b * rhs, (float)a * rhs);
}

void pxl::Color::preMultiply()
{
	r = r * a / 255;
	g = g * a / 255;
	b = b * a / 255;
}

const pxl::Color pxl::Color::transparent = pxl::Color(0, 0, 0, 0);
const pxl::Color pxl::Color::white = pxl::Color(255, 255, 255, 255);
const pxl::Color pxl::Color::black = pxl::Color(0, 0, 0, 255);
const pxl::Color pxl::Color::red = pxl::Color(255, 0, 0, 255);
const pxl::Color pxl::Color::green = pxl::Color(0, 255, 0, 255);
const pxl::Color pxl::Color::blue = pxl::Color(0, 0, 255, 255);
const pxl::Color pxl::Color::yellow = pxl::Color(255, 255, 0, 255);
const pxl::Color pxl::Color::purple = pxl::Color(255, 0, 255, 255);
const pxl::Color pxl::Color::teal = pxl::Color(0, 255, 255, 255);
