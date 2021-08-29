#pragma once
#include <pxl/containers/string.h>
#include <pxl/types.h>

namespace pxl
{
	class Audio
	{
	public:
		Audio(const String& file);
		Audio(Audio&& src) noexcept;
		Audio& operator=(Audio&& src) noexcept;
		Audio& operator=(const Audio& src);
		Audio();
		~Audio();

		int channels() const;
		int samples() const;
		const u16* data() const;
		int size() const;
	private:
		int _samples;
		int _channels;
		int _size;
		short *_data;
	};
}