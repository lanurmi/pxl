#pragma once
#include <memory>
#include <pxl/containers/string.h>
#include <pxl/assets/image.h>
#include <pxl/types.h>

namespace pxl
{
	namespace video
	{
		enum class Codec
		{
			MPEG4,
			H264
		};

		struct FileInfo
		{
			pxl::i64 duration;
			pxl::i64 frames;
			int width;
			int height;
			double framerate;
			int bitrate;
			int gop_size;
			pxl::String file;
		};

		class Encoder
		{
		public:
			Encoder(const Encoder&) = delete;
			Encoder(Encoder&) = delete;
			Encoder& operator=(const Encoder&) = delete;
			Encoder& operator=(Encoder&&) = delete;
			virtual ~Encoder(){}
			virtual void add(const pxl::Image& image) = 0;
			virtual void save() = 0;
		protected:
			Encoder(){}
		};

		struct EncoderInfo
		{
			int width;
			int height;
			double framerate;
			int bitrate;
			int gop_size = 10;
			int b_frames = 1;
			pxl::String file;
			Codec codec = Codec::MPEG4;
		};
		using EncoderRef = std::shared_ptr<Encoder>;
		EncoderRef createEncoder(const EncoderInfo &info);


		class Decoder {
		public:
			Decoder(const Decoder&) = delete;
			Decoder(Decoder&) = delete;
			Decoder& operator=(const Decoder&) = delete;
			Decoder& operator=(Decoder&&) = delete;

			virtual ~Decoder() {};
			virtual void open(const pxl::String &file) = 0;
			virtual void close() = 0;
			virtual void preProcess() = 0;
			virtual pxl::Image currentFrameImage() = 0;
			virtual pxl::i64 currentFrameNumber() const = 0;
			virtual void flushDecoder() = 0;
			virtual bool seek(int seekFrame) = 0;
			virtual bool decode() = 0;
			virtual FileInfo info() const = 0;
		protected:
			Decoder() {}
		};

		using DecoderRef = std::shared_ptr<Decoder>;
		DecoderRef createDecoder();
	}
}
