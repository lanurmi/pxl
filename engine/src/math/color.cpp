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

bool pxl::Color::operator==(const Color& color) const
{
	return r == color.r && g == color.g && b == color.b && a == color.a;
}

bool pxl::Color::operator!=(const Color& color) const
{
	return r != color.r || g != color.g || b != color.b || a != color.a;
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
