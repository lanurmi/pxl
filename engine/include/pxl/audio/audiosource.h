#pragma once

#include <future>
#include <pxl/assets/audio.h>
#include <pxl/audio/audiobuffer.h>

namespace pxl
{
	class AudioSource;
	using AudioSourceRef = std::shared_ptr<AudioSource>;

	class AudioSource
	{
	public:
		virtual ~AudioSource(){}
		static AudioSourceRef create();
		virtual void bind(AudioBufferRef buffer) = 0;
		virtual void setGain(float gain) = 0;
		virtual bool isPlaying() const = 0;
		virtual void play() = 0;
	protected:
		AudioSource();
	};
}
