#pragma once
#include <functional>
#include <pxl/containers/vector.h>
#include <pxl/audio/audiosource.h>
#include <pxl/audio/audiobuffer.h>

namespace pxl
{
	namespace audio 
	{
		void init();
		void shutdown();
		AudioSourceRef createAudioSource();
		AudioBufferRef createAudioBuffer(const Audio& clip);
	}
}