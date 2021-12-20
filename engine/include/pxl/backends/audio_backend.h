#pragma once
#include <functional>
#include <pxl/containers/vector.h>
#include <pxl/audio/audiosource.h>
#include <pxl/audio/audiobuffer.h>

namespace pxl
{
	namespace audio 
	{
		void awake();
		void destroy();
		AudioSourceRef createAudioSource();
		AudioBufferRef createAudioBuffer(const Audio& clip);
	}
}

