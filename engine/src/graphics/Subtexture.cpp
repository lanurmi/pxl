#include <pxl/graphics/Subtexture.h>

using namespace pxl;

Subtexture::Subtexture(const TextureRef& texture) : _texture(texture)
{
}

Subtexture::Subtexture(const TextureRef& texture, const Rect& rect)
{
	set(texture, rect);
}

void Subtexture::set(const TextureRef& texture, const Rect& rect)
{
	_texture = texture;
	_rect = rect;
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

Rect Subtexture::textureCoordinates() const
{
	return pxl::Rect(_rect.x / _texture->width(), _rect.y / _texture->width(), _rect.width / _texture->width(), _rect.height / _texture->height());
}

int Subtexture::width() const
{
	return _rect.width;
}

int Subtexture::height() const
{
	return _rect.height;
}

int Subtexture::x() const
{
	return _rect.x;
}

int Subtexture::y() const
{
	return _rect.y;
}

