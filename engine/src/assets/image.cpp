#include <pxl/assets/image.h>
#include <pxl/engine.h>
#include <pxl/utils/filestream.h>
#include <pxl/assets/imageformats.h>
#include <assert.h>



using namespace pxl;

pxl::Image::Image(int width, int height) : _pixels(nullptr), _width(width), _height(height)
{
	assert(width > 0 && height > 0);
	_pixels = new Color[_width * _height];
	memset(_pixels, 0, sizeof(Color) * _width * _height);
}


pxl::Image::Image(const String& file)
{
	load(file);
}

pxl::Image::Image() : _width(0), _height(0), _pixels(nullptr)
{

}

pxl::Image::Image(const Image& src)
{
	_width = src._width;
	_height = src._height;
	if (src._pixels != nullptr && _width > 0 && _height > 0)
	{
		_pixels = new Color[_width * _height];
		memcpy(_pixels, src._pixels, sizeof(Color) * _width * _height);
	}
}

pxl::Image::Image(Image&& src) noexcept
{
	_width = src._width;
	_height = src._height;
	_pixels = src._pixels;
	src._pixels = nullptr;
}

pxl::Image& pxl::Image::operator=(Image&& src) noexcept
{
	assert(_pixels == nullptr);
	delete[] _pixels;
	_width = src._width;
	_height = src._height;
	_pixels = src._pixels;
	src._pixels = nullptr;
	return *this;
}

pxl::Image& pxl::Image::operator=(const Image& src)
{
	pxl::Image img(src);
	std::swap(*this, img);
	return *this;
}

pxl::Image::~Image()
{
	delete [] _pixels;
}

void pxl::Image::load(const pxl::String& file)
{
	FileStream filestream(file, file::FileMode::ReadBinary);

	_pixels = pxl::imageformats::decode(filestream, &_width, &_height);

	assert(_width > 0);
	assert(_height > 0);
	assert(_pixels != nullptr);
}

int pxl::Image::width() const
{
	return _width;
}

int pxl::Image::height() const
{
	return _height;
}

void pxl::Image::setPixels(const pxl::Color* pixels)
{
	memcpy(_pixels, pixels, sizeof(pxl::Color) * _width * _height);
}

void  pxl::Image::setPixels(const Color& color)
{
	for(int i = 0; i < _width * _height; i++)
	{
		_pixels[i] = color;
	}
}

void pxl::Image::setPixels(const pxl::Rect& rect, const Color* data)
{
	for (int i = 0; i < rect.width; i++)
	{
		for (int j = 0; j < rect.height; j++)
		{
			int srcIdx = j * rect.width + i;
			int dstIdx = (rect.y + j) * _width + (i + rect.x);
			auto src = data[srcIdx];
			_pixels[dstIdx] = src;
		}
	}
}


void pxl::Image::setPixels(const pxl::Rect& rect, const Color& color)
{
	for (int i = 0; i < rect.width; i++)
	{
		for (int j = 0; j < rect.height; j++)
		{
			int dstIdx = (rect.y + j) * _width + (i + rect.x);
			_pixels[dstIdx] = color;
		}
	}
}

pxl::Color* pxl::Image::pixels() const
{
	return _pixels;
}

void pxl::Image::preMultiply()
{
	assert(_pixels);
	for (int i = 0; i < _width * _height; i++)
	{
		(_pixels + i)->preMultiply();
	}
}