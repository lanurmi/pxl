#pragma once
#include <pxl/math/color.h>
#include <pxl/math/rect.h>
#include <pxl/types.h>

namespace pxl
{
	class Image
	{
	public:
		Image(int width, int height);
		Image(const String &file);
		Image(const Image& src);
		Image(Image &&src) noexcept;
		Image& operator=(Image&& src) noexcept;
		Image& operator=(const Image& src);
		Image();
		~Image();
		void load(const String& file);
		int width() const;
		int height() const;
		void setPixels(const Color* data);
		void setPixels(const Color &color);
		void setPixels(const pxl::Rect& rect, const Color* data);
		void setPixels(const pxl::Rect& rect, const Color& color);
		Color* pixels() const;
		void preMultiply();
	private:
		int _width;
		int _height;
		Color* _pixels;
	};
}