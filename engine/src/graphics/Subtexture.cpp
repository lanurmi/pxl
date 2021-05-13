#include <pxl/graphics/Subtexture.h>

using namespace pxl;

Subtexture::Subtexture(const TextureRef& texture) : _texture(texture)
{
}

Subtexture::Subtexture(const TextureRef& texture, const Rect& rect) : _texture(texture)
{
	set(rect);
}

void Subtexture::set(const Rect& rect)
{
	_rect = rect;
}

TextureRef Subtexture::texture() const
{
	return _texture;
}

Rect Subtexture::rect() const
{
	return _rect;
}

int Subtexture::width() const
{
	return _rect.width;
}

int Subtexture::height() const
{
	return _rect.height;
}
