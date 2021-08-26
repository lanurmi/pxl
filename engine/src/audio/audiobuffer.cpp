#include <pxl/audio/audiobuffer.h>
#include <pxl/backends/audio_backend.h>

using namespace pxl;

AudioBufferRef AudioBuffer::create(const Audio &audio)
{
	return pxl::audio::createAudioBuffer(audio);
}

AudioBuffer::AudioBuffer()
{

}
