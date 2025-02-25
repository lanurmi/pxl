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
			Codec codec;
			bool seekToPts;
		};

		class Frame
		{
		public:
			Frame(const Frame&) = delete;
			Frame& operator=(const Frame&) = delete;
			Frame& operator=(Frame&&) = delete;
			virtual ~Frame() {}
			virtual const pxl::Image& rgba() const = 0;
			virtual pxl::i64 frame() const = 0;
		protected:
			Frame() {}
		};

		using FrameRef = std::shared_ptr<Frame>;

		class Encoder
		{
		public:
			Encoder(const Encoder&) = delete;
			Encoder& operator=(const Encoder&) = delete;
			Encoder& operator=(Encoder&&) = delete;
			virtual ~Encoder() {}
			virtual bool isOk() const = 0;
			virtual void add(const FrameRef& image) = 0;
			virtual void save() = 0;
		protected:
			Encoder() {}
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
		EncoderRef createEncoder(const EncoderInfo& info);

		struct Progress
		{
			pxl::i64 max;
			pxl::i64 current;
			pxl::Func<> update;
		};

		class Decoder {
		public:
			Decoder(const Decoder&) = delete;
			Decoder& operator=(const Decoder&) = delete;
			Decoder& operator=(Decoder&&) = delete;

			virtual ~Decoder() {};
			virtual void open(const pxl::String& file) = 0;
			virtual void close() = 0;
			virtual void calculateFrames(Progress& progress) = 0;
			virtual FrameRef currentFrame() = 0;
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
		FrameRef createFrame(const pxl::Image& image, pxl::i64 frame);
	}
}
