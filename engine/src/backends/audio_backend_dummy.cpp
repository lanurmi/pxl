#ifdef PXL_AUDIO_DUMMY

#include <pxl/engine.h>
#include <pxl/backends/audio_backend.h>

using namespace pxl;

class DummyAudioBuffer : public AudioBuffer {
public:
	DummyAudioBuffer()
	{
	}
	virtual ~DummyAudioBuffer()
	{
	}
private:
};

class DummyAudioSource : public AudioSource {
public:
	OpenALAudioSource()
	{
	}
	virtual ~OpenALAudioSource()
	{
	}
	void bind(AudioBufferRef)
	{
	}
	void setGain(float) override
	{
	}
	void play() override 
	{
	}
	bool isPlaying() const override
	{
		return false;
	}
};


void audio::awake()
{
}

void audio::destroy()
{
}

AudioSourceRef audio::createAudioSource()
{
	return nullptr;
}

AudioBufferRef audio::createAudioSource(const Audio&)
{
	return nullptr;
}

#endif
