#include <pxl/assets/audio.h>
#include <pxl/backends/filesystem_backend.h>
#include <pxl/assets/audioformats.h>
#include <pxl/log.h>

using namespace pxl;

Audio::Audio(const String& file) : _data(nullptr), _size(0), _samples(0), _channels(0)
{
	pxl::FileStream stream(file, file::FileMode::ReadBinary);
	_data = pxl::audioformats::decode(stream, _size, _samples, _channels);
}

Audio::Audio() : _data(nullptr), _size(0), _samples(0), _channels(0) {

}

Audio::Audio(const Audio& src) : _data(nullptr), _size(0), _samples(0), _channels(0) {
	_data = new short[src._size];
	memcpy(_data, src._data, src._size);
	_samples = src._samples;
	_channels = src._channels;
	_size = src._size;
}

Audio::Audio(Audio&& src) noexcept : _data(nullptr), _size(0), _samples(0), _channels(0) {
	_data = src._data;
	_samples = src._samples;
	_channels = src._channels;
	_size = src._size;

	src._data = nullptr;
	src._size = 0;
	src._samples = 0;
	src._channels = 0;
}

Audio& Audio::operator=(Audio&& src) noexcept {
	_data = src._data;
	_samples = src._samples;
	_channels = src._channels;
	_size = src._size;

	src._data = nullptr;
	src._size = 0;
	src._samples = 0;
	src._channels = 0;
	return *this;
}

Audio& Audio::operator=(const Audio& src) {

	Audio copy(src);
	std::swap(*this, copy);
	return *this;
}

Audio::~Audio() {
	delete [] _data;
}


int Audio::channels() const {
	return _channels;
}
int Audio::samples() const {
	return _samples;
}

const u16* Audio::data() const {
	return (u16*)_data;
}
int Audio::size() const {
	return _size;
}

