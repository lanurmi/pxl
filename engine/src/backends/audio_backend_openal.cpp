#ifdef PXL_AUDIO_OPENAL

#include <pxl/engine.h>
#include <pxl/backends/audio_backend.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <cassert>

using namespace pxl;

class OpenALAudioBuffer : public AudioBuffer
{
public:
	OpenALAudioBuffer() : _buffer(0)
	{
	}
	virtual ~OpenALAudioBuffer()
	{
	}
	void setData(const u16* data, int size, int channels, int samples)
	{
		alGenBuffers(1, &_buffer);
		int alFormat = AL_NONE;
		if (channels == 1) 
		{
			alFormat = AL_FORMAT_MONO16;
		}
		else if (channels == 2)
		{
			alFormat = AL_FORMAT_STEREO16;
		} 
		else
		{
			pxl::log::error("OpenAL implementation does not support more than 2 channels per clip");
			assert(0);
		}
		alBufferData(_buffer, alFormat, data, size, samples);
	}
	ALuint buffer() const
	{
		return _buffer;
	}
private:
	ALuint _buffer;
};

class OpenALAudioSource : public AudioSource
{
public:
	OpenALAudioSource() : _source(0)
	{
		create();
	}
	void create()
	{
		alGenSources(1, &_source);
		alSourcef(_source, AL_PITCH, 1.0f);
		alSourcef(_source, AL_GAIN, 1.0f);
		alSource3f(_source, AL_POSITION, 0, 0, 0);
		alSource3f(_source, AL_VELOCITY, 0, 0, 0);
		alSourcei(_source, AL_LOOPING, false);
	}
	virtual ~OpenALAudioSource()
	{
		alDeleteSources(1, &_source);
	}
	void bind(AudioBufferRef buffer) override
	{
		auto b = std::dynamic_pointer_cast<OpenALAudioBuffer>(buffer);
		alSourcei(_source, AL_BUFFER, b->buffer());
	}
	void setGain(float gain) override
	{
		alSourcef(_source, AL_GAIN, gain);
	}
	void play() override
	{
		alSourcePlay(_source);
	}
	bool isPlaying() const override
	{
		ALenum state;
		alGetSourcei(_source, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}
	ALuint buffer() const
	{
		return _source;
	}
private:
	ALuint _source;
};

namespace
{
	ALCdevice *s_device;
	ALCcontext *s_context;
}

void audio::init()
{
	s_device = alcOpenDevice(nullptr);
	s_context = alcCreateContext(s_device, nullptr);
	alcMakeContextCurrent(s_context);
}

void audio::shutdown()
{
	alcDestroyContext(s_context);
	alcCloseDevice(s_device);
}

AudioBufferRef audio::createAudioBuffer(const Audio& audio)
{
	auto buffer = std::make_shared<OpenALAudioBuffer>();
	buffer->setData(audio.data(), audio.size(), audio.channels(), audio.samples());
	return buffer;
}

AudioSourceRef audio::createAudioSource()
{
	return AudioSourceRef(new OpenALAudioSource());
}

#endif
