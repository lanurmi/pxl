#include <pxl/assets/audio.h>
#include <pxl/log.h>
#include <pxl/3rdparty/stb_vorbis.h>
#include <pxl/backends/filesystem_backend.h>

using namespace pxl;

Audio::Audio(const String& file) : _data(nullptr), _size(0), _samples(0), _channels(0)
{
	auto f = file::File::open(file, file::FileMode::ReadBinary);
	u8 *decoded = new u8[f->length()];
	f->read(decoded, f->length());
	_size = stb_vorbis_decode_memory(decoded, f->length(), &_channels, &_samples, &_data);
	if (_size <= 0) {
		pxl::log::error("Could not load audio file");
	}
	delete [] decoded;
}

Audio::Audio() : _data(nullptr), _size(0), _samples(0), _channels(0) {

}

Audio::Audio(const Audio& src) : _data(nullptr), _size(0), _samples(0), _channels(0) {
	_data = (short*)malloc(src._size);
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
	free(_data);
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