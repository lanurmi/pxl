#include <pxl/image/image.h>
#include <pxl/platforms/platform_backend.h>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include <pxl/3rdparty/stb_image.h>

pxl::Image::Image(int width, int height) : _pixels(nullptr), _width(width), _height(height)
{
	assert(width > 0 && height == 0);
	_pixels = new Color[_width * _height];
}

static pxl::Color* loadJPEG(const pxl::string& path, int *w, int *h)
{
	int comp;
	auto data = stbi_load(path.c_str(), w, h, &comp, STBI_rgb);
	if (data == nullptr)
	{
		assert(0);
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
		assert(0);
	}
	auto size = (*w) * (*h);
	auto result = new pxl::Color[size];
	memcpy(result, data, size * sizeof(pxl::Color));
	stbi_image_free(data);
	return result;
}

pxl::Image::Image(const string& file)
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
}

pxl::Image::~Image()
{
	delete [] _pixels;
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