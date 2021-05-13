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
		Image(const string &file);
		Image(Image &&src) noexcept;
		Image& operator=(Image&& src) noexcept;
		Image& operator=(const Image& src);
		Image();
		~Image();
		void load(const string& file);
		int width() const;
		int height() const;
		void setPixels(Color* data);
		void setPixels(const pxl::Rect& rect, Color* data);
		Color* pixels() const;
	private:
		void preMultiply();
		int _width;
		int _height;
		Color* _pixels;
	};
}