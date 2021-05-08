#pragma once
#include <pxl/math/color.h>
#include <pxl/types.h>

namespace pxl
{
	class Image
	{
	public:
		Image(int width, int height);
		Image(const string &file);
		~Image();
		int width() const;
		int height() const;
		void setPixels(Color* data);
		Color* pixels() const;
	private:
		void preMultiply();
		int _width;
		int _height;
		Color* _pixels;
	};
}