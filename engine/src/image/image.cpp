#include <pxl/image/image.h>
#include <pxl/engine.h>
#include <pxl/filesystem.h>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include <pxl/3rdparty/stb_image.h>

pxl::Image::Image(int width, int height) : _pixels(nullptr), _width(width), _height(height)
{
	assert(width > 0 && height > 0);
	_pixels = new Color[_width * _height];
}

static pxl::Color* loadJPEG(const pxl::string& path, int *w, int *h)
{
	int comp;
	auto data = stbi_load(path.c_str(), w, h, &comp, STBI_rgb);
	if (data == nullptr)
	{
		pxl::log().error("Could not load rgb image");
		assert(0);
		return nullptr;
	}
	auto size = (*w) * (*h);
	auto result = new pxl::Color[size];
	for (int i = 0; i < size; i++)
	{
		auto r = data[3 * i + 0];
		auto g = data[3 * i + 1];
		auto b = data[3 * i + 2];
		auto a = 255;

		result[i].r = r;
		result[i].g = g;
		result[i].b = b;
		result[i].a = a;
	}
	stbi_image_free(data);
	return result;
}

static pxl::Color* loadPNG(const pxl::string& path, int* w, int* h)
{
	int comp;
	auto data = stbi_load(path.c_str(), w, h, &comp, STBI_rgb_alpha);
	if (data == nullptr)
	{
		pxl::log().error("Could not load png image");
		assert(0);
		return nullptr;
	}
	auto size = (*w) * (*h);
	auto result = new pxl::Color[size];
	memcpy(result, data, size * sizeof(pxl::Color));
	stbi_image_free(data);
	return result;
}

pxl::Image::Image(const string& file)
{
	load(file);
}

pxl::Image::Image() : _width(0), _height(0), _pixels(nullptr)
{

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
	delete[] _pixels;
	_width = src._width;
	_height = src._height;
	if (src._pixels != nullptr && _width > 0 && _height > 0)
	{
		_pixels = new Color[_width * _height];
		memcpy(_pixels, src._pixels, sizeof(Color) * _width * _height);
	}
	return *this;
}

pxl::Image::~Image()
{
	delete [] _pixels;
}

void pxl::Image::load(const pxl::string& file)
{
	auto ext = pxl::path::extension(file);
	if (ext == ".png")
	{
		_pixels = loadPNG(file, &_width, &_height);
		preMultiply();
	}
	else if (ext == ".jpg" || ext == ".jpeg")
	{
		_pixels = loadJPEG(file, &_width, &_height);
	}

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

void pxl::Image::setPixels(pxl::Color* pixels)
{
	memcpy(_pixels, pixels, sizeof(pxl::Color) * _width * _height);
}

void pxl::Image::setPixels(const pxl::Rect& rect, Color* data)
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