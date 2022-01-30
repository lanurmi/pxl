#pragma once
#include <pxl/math/rect.h>
#include <pxl/graphics/texture.h>

namespace pxl
{
	class Subtexture
	{
	public:
		Subtexture() = default;
		Subtexture(const TextureRef& texture);
		Subtexture(const TextureRef& texture, const Rect &rect);
		void set(const TextureRef& texture, const Rect& rect);
		void set(const Rect& rect);
		TextureRef texture() const;
		Rect rect() const;
		Rect textureCoordinates() const;
		int width() const;
		int height() const;
		int x() const;
		int y() const;
	private:
		TextureRef _texture;
		Rect _rect;
	};
}

