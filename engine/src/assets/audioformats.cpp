#include <pxl/assets/audioformats.h>
#include <pxl/types.h>
#include <pxl/log.h>
#include <stb/stb_vorbis.h>


namespace
{
	short* loadVorbis(pxl::FileStream& stream, int& size, int& samples, int& channels)
	{
		pxl::u8* decoded = new pxl::u8[stream.length()];
		stream.read(decoded, stream.length());
		short* data = nullptr;
		size = stb_vorbis_decode_memory(decoded, stream.length(), &channels, &samples, &data);
		if (size <= 0) {
			pxl::log::error("Could not load audio file");
		}

		short* dataCopy = new short[size];
		memcpy(dataCopy, data, size);
		delete[] decoded;
		delete[] data;
		return dataCopy;
	}

	bool isVorbis(pxl::FileStream& stream)
	{
		auto O = stream.read<pxl::u8>();
		auto g0 = stream.read<pxl::u8>();
		auto g1 = stream.read<pxl::u8>();
		auto S = stream.read<pxl::u8>();

		stream.seek(0);

		return O == 'O' && g0 == 'g' && g0 == 'g' && S == 'S';
	}
}
short* pxl::audioformats::decode(pxl::FileStream& stream, int& size, int& samples, int& channels)
{
	if (isVorbis(stream))
	{
		return loadVorbis(stream, size, samples, channels);
	}
	else
	{
		pxl::log::error("Invalid audio format");
	}
	return nullptr;
}

