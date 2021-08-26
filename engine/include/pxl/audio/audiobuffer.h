#pragma once

#include <future>
#include <pxl/assets/audioclip.h>

namespace pxl
{
	class AudioBuffer;
	using AudioBufferRef = std::shared_ptr<AudioBuffer>;

	class AudioBuffer
	{
	public:
		virtual ~AudioBuffer() {}
		static AudioBufferRef create(const Audio& audio);
	protected:
		AudioBuffer();
	};
}