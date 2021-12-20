#include <pxl/assets/imageformats.h>
#include <pxl/log.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include <stb/stb_image.h>

using namespace pxl;

Color* loadJPEG(FileStream& stream, int* w, int* h) {
	u8* srcData = new u8[stream.length()];
	stream.read(srcData, stream.length());
	int comp;
	auto data = stbi_load_from_memory(srcData, stream.length(), w, h, &comp, STBI_rgb);
	delete[] srcData;
	if (data == nullptr) 	{
		pxl::log::error("Could not load rgb image");
		assert(0);
		return nullptr;
	}
	auto size = (*w) * (*h);
	auto result = new pxl::Color[size];
	for (int i = 0; i < size; i++) 	{
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

bool IsPng(FileStream& stream) {
	int position = stream.position();
	u8 firstByte = stream.read<u8>();
	u8 p = stream.read<u8>();
	u8 n = stream.read<u8>();
	u8 g = stream.read<u8>();
	stream.seek(position);
	return firstByte == 0x89 && p == 0x50 && n == 0x4e && g == 0x47;
}

pxl::Color* loadPNG(FileStream& stream, int* w, int* h) {

	u8* srcData = new u8[stream.length()];
	stream.read(srcData, stream.length());
	int comp;
	auto data = stbi_load_from_memory(srcData, stream.length(), w, h, &comp, STBI_rgb_alpha);
	delete[] srcData;
	if (data == nullptr) {
		pxl::log::error("Could not load png image");
		assert(0);
		return nullptr;
	}
	auto size = (*w) * (*h);
	auto result = new pxl::Color[size];
	memcpy(result, data, size * sizeof(pxl::Color));
	stbi_image_free(data);
	return result;
}


Color* imageformats::decode(FileStream& stream, int* w, int* h) {
	if (IsPng(stream)) 	{
		return loadPNG(stream, w, h);
	}
	else
	{
		return loadJPEG(stream, w, h);
	}
}

