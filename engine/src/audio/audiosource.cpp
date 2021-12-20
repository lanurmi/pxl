#include <pxl/audio/audiosource.h>
#include <pxl/backends/audio_backend.h>

using namespace pxl;

AudioSourceRef AudioSource::create()
{
	return pxl::audio::createAudioSource();
}

AudioSource::AudioSource()
{

}

