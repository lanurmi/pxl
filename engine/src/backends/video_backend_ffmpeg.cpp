#if PXL_VIDEO_FFMPEG
#include <pxl/backends/video_backend.h>
#include <pxl/backends/platform_backend.h>


extern "C"
{
	#ifndef __STDC_CONSTANT_MACROS
	#define __STDC_CONSTANT_MACROS
	#endif

	#include <libavcodec/avcodec.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/avstring.h>
	#include <libavutil/time.h>
	#include <libavutil/opt.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
}

namespace
{
	bool s_ffmpeg_inited = false;
}

struct FrameInfo {
	pxl::i64 pts;
	pxl::i64 dts;
	int frameNumber;
};


class Encoder_ffmpeg : public pxl::video::Encoder
{
public:
	Encoder_ffmpeg(const pxl::video::EncoderInfo& info);
	~Encoder_ffmpeg();
	void add(const pxl::Image& image) override;
	void save() override;
private:
	pxl::video::EncoderInfo info;
	AVFrame* getYuv(const pxl::Image& image);
	AVCodecContext* codecCtx;
	AVFrame* frame;
	AVFrame* frameRgba;
	AVFrame* frameYuv;
	AVOutputFormat* format;
	AVFormatContext* formatContext;
	AVStream* stream;
	pxl::u8* buffer;
	pxl::i64 pts;
};

static AVCodecID pxlToFFmpeg(pxl::video::Codec codec)
{
	switch(codec)
	{
		case pxl::video::Codec::MPEG4: return AV_CODEC_ID_MPEG4;
		case pxl::video::Codec::H264: return AV_CODEC_ID_H264;
	}
	assert(0);
	return AV_CODEC_ID_MPEG4;
}

Encoder_ffmpeg::Encoder_ffmpeg(const pxl::video::EncoderInfo& info) : info(info) {
	if (!s_ffmpeg_inited)
	{
		av_register_all();
		s_ffmpeg_inited = true;
	}

	codecCtx = nullptr;
	frame = nullptr;
	frameRgba = nullptr;

	frame = av_frame_alloc();
	frameRgba = av_frame_alloc();
	frameYuv = av_frame_alloc();
	buffer = nullptr;

	//
	format = av_guess_format(nullptr, info.file.cstr(), nullptr);
	if (format == nullptr)
	{
		assert(0);
	}
	if (avformat_alloc_output_context2(&formatContext, format, nullptr, info.file.cstr()) < 0)
	{
		assert(0);
	}


	AVCodec* codec = avcodec_find_encoder(pxlToFFmpeg(info.codec));
	if (!codec) 
	{
		assert(0);
	}

	stream = avformat_new_stream(formatContext, codec);

	if (!stream)
	{
		assert(0);
	}

	codecCtx = avcodec_alloc_context3(codec);


	stream->codecpar->codec_id = pxlToFFmpeg(info.codec);
	stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
	stream->codecpar->width = info.width;
	stream->codecpar->height = info.height;
	stream->codecpar->format = AV_PIX_FMT_YUV420P;
	stream->codecpar->bit_rate = info.bitrate;

	stream->avg_frame_rate = av_d2q(info.framerate, 100);

	avcodec_parameters_to_context(codecCtx, stream->codecpar);
	codecCtx->time_base = av_inv_q( av_d2q(info.framerate, 100) );

	codecCtx->max_b_frames = info.b_frames;
	codecCtx->gop_size = info.gop_size;
	codecCtx->framerate = av_d2q(info.framerate, 100);

	auto cpus = pxl::platform::cpuCount();
	if (cpus > -1) {
		codecCtx->thread_count = cpus;
	}

	if (stream->codecpar->codec_id == AV_CODEC_ID_H264)
	{
		codecCtx->profile = FF_PROFILE_H264_MAIN;
	}
	else if(stream->codecpar->codec_id == AV_CODEC_ID_MPEG4)
	{
		codecCtx->mb_decision = FF_MB_DECISION_RD; // -mbd rd
		codecCtx->flags |= AV_CODEC_FLAG_4MV; // -flags +mv4 ???
		codecCtx->flags |= AV_CODEC_FLAG_PASS1; // -pass 1, PASS2 ei toimi
		codecCtx->trellis = 2; // -trellis 2
		codecCtx->mv0_threshold = 2; // -cmp 2 // -cmp <string or int> : Full pel motion estimation compare function.
		codecCtx->me_subpel_quality = 2; // -subcmp 2
		codecCtx->gop_size = info.gop_size; // -g 300
	}

	avcodec_parameters_from_context(stream->codecpar, codecCtx);

	if (avcodec_open2(codecCtx, codec, nullptr) < 0)
	{
		assert(0);
	}

	if (!(format->flags & AVFMT_NOFILE))
	{
		if ((avio_open(&formatContext->pb, info.file.cstr(), AVIO_FLAG_WRITE)) < 0)
		{
			assert(0);
		}
	}

	if (avformat_write_header(formatContext, nullptr) < 0)
	{
		assert(0);
	}

	pts = 0;

	frameYuv->width = codecCtx->width;
	frameYuv->height = codecCtx->height;
	frameYuv->format = codecCtx->pix_fmt;

	if (av_frame_get_buffer(frameYuv, 32) < 0)
	{
		assert(0);
	}
}

Encoder_ffmpeg::~Encoder_ffmpeg()
{

	av_frame_free(&frame);
	av_free(frame);



	av_frame_free(&frameRgba);
	av_free(frameRgba);

	av_frame_free(&frameYuv);
	av_free(frameYuv);

	av_free(buffer);

	avformat_free_context(formatContext);

	avcodec_close(codecCtx);

	buffer = nullptr;
	frameRgba = nullptr;
	frameYuv = nullptr;
	frame = nullptr;
	codecCtx = nullptr;
}

void Encoder_ffmpeg::add(const pxl::Image& image) 
{
	auto yuvFrame = getYuv(image);

	yuvFrame->pts = pts * (pxl::i64)stream->time_base.den / (stream->time_base.num * info.framerate);
	auto ret = avcodec_send_frame(codecCtx, yuvFrame);
	if (ret < 0)
	{
		assert(0);
	}


	AVPacket packet;
	av_init_packet(&packet);
	packet.data = nullptr;
	packet.size = 0;

	while (ret >= 0)
	{
		ret = avcodec_receive_packet(codecCtx, &packet);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
		{
			break;
		} 	
		else if (ret < 0)
		{
			assert(0);
		}

		if(codecCtx->coded_frame->key_frame) {
			packet.flags |= AV_PKT_FLAG_KEY;
		}

		av_interleaved_write_frame(formatContext, &packet);
	}
	av_packet_unref(&packet);
	pts++;
}

AVFrame* Encoder_ffmpeg::getYuv(const pxl::Image& image)
{
	if (av_frame_make_writable(frameYuv) < 0)
	{
		assert(0);
	}

	frameRgba->format = AV_PIX_FMT_RGBA;
	frameRgba->width = image.width();
	frameRgba->height = image.height();

	frameRgba->data[0] = &image.pixels()->r;
	frameRgba->linesize[0] = image.width() * 4;

	if (buffer == nullptr) {
		int numBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecCtx->width, codecCtx->height, 32);
		buffer = (pxl::u8*)av_malloc(numBytes * sizeof(pxl::u8));
	}

	av_image_fill_arrays(
		frameYuv->data,
		frameYuv->linesize,
		buffer,
		AV_PIX_FMT_YUV420P,
		codecCtx->width,
		codecCtx->height,
		32
	);

	struct SwsContext* sws_ctx = nullptr;

	sws_ctx = sws_getContext(
		image.width(),
		image.height(),
		AV_PIX_FMT_RGBA,
		codecCtx->width,
		codecCtx->height,
		codecCtx->pix_fmt,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);

	sws_scale(
		sws_ctx,
		(uint8_t const* const*)frameRgba->data,
		frameRgba->linesize,
		0,
		codecCtx->height,
		frameYuv->data,
		frameYuv->linesize
	);

	sws_freeContext(sws_ctx);
	return frameYuv;
}

void Encoder_ffmpeg::save()
{
	AVPacket packet;
	av_init_packet(&packet);
	packet.data = nullptr;
	packet.size = 0;

	auto ret = avcodec_send_frame(codecCtx, nullptr);
	if (ret < 0)
	{
		assert(0);
	}

	while (ret >= 0)
	{
		ret = avcodec_receive_packet(codecCtx, &packet);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
		{
			break;
		}
		else if (ret < 0)
		{
			assert(0);
		}

		if (codecCtx->coded_frame->key_frame) {
			packet.flags |= AV_PKT_FLAG_KEY;
		}

		av_interleaved_write_frame(formatContext, &packet);
	}

	av_packet_unref(&packet);
	av_write_trailer(formatContext);

	if (!(format->flags & AVFMT_NOFILE))
	{
		int err = avio_close(formatContext->pb);
		if (err < 0)
		{
			assert(0);
		}
	}
}

pxl::video::EncoderRef pxl::video::createEncoder(const EncoderInfo &info)
{
	return std::make_shared<Encoder_ffmpeg>(info);
}

class Decoder_ffmpeg : public pxl::video::Decoder
{
public:
	Decoder_ffmpeg();
	~Decoder_ffmpeg();
	void open(const pxl::String &file) override;
	void close() override;
	void preProcess() override;
	pxl::Image currentFrameImage() override;
	pxl::i64 currentFrameNumber() const override;
	void flushDecoder() override;
	bool seek(int seekFrame)  override;
	bool decode() override;
	pxl::video::FileInfo info() const override;
private:
	bool decode(FrameInfo* fillInfo);
	pxl::i64 seekTarget;
	pxl::i64 seekTargetFrame;
	AVFormatContext* formatCtx;;
	AVCodecContext* codecCtxOrig;
	AVCodecContext* codecCtx;
	AVFrame* frame;
	AVFrame* frameRgba;
	pxl::u8* buffer;
	pxl::Vector<int> videoStreamIds;
	pxl::Vector<int> audioStreamIds;
	int videoStreamId;
	pxl::i64 currentFrame;
	FrameInfo currentFrameInfo;
	pxl::Map<pxl::i64, pxl::i64> dtsToTime;
	pxl::String file;
};

Decoder_ffmpeg::Decoder_ffmpeg() : seekTarget(-1), seekTargetFrame(0),
formatCtx(nullptr), codecCtxOrig(nullptr), codecCtx(nullptr), frame(nullptr), frameRgba(nullptr), buffer(nullptr), videoStreamId(-1), currentFrame(0) {
	if (!s_ffmpeg_inited)
	{
		av_register_all();
		avcodec_register_all();
		s_ffmpeg_inited = true;
	}
}

Decoder_ffmpeg::~Decoder_ffmpeg()
{
	close();
}

void Decoder_ffmpeg::open(const pxl::String& file)
{
	this->file = file;
	frame = av_frame_alloc();
	frameRgba = av_frame_alloc();

	if (avformat_open_input(&formatCtx, file.cstr(), nullptr, nullptr) < 0)
	{
		assert(0);
	}
	if (avformat_find_stream_info(formatCtx, nullptr) < 0)
	{
		assert(0);
	}
	for (unsigned i = 0; i < formatCtx->nb_streams; i++)
	{
		if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStreamIds.add(i);
			break;
		}
		else if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStreamIds.add(i);
			break;
		}
	}
	videoStreamId = videoStreamIds.front();

	if (videoStreamId == -1)
	{
		assert(0);
	}
	AVCodec* codec = nullptr;
	codec = avcodec_find_decoder(formatCtx->streams[videoStreamId]->codecpar->codec_id);
	if (codec == nullptr)
	{
		assert(0);
	}
	codecCtxOrig = avcodec_alloc_context3(codec);
	if (avcodec_parameters_to_context(codecCtxOrig, formatCtx->streams[videoStreamId]->codecpar) != 0)
	{
		assert(0);
	}
	codecCtx = avcodec_alloc_context3(codec);
	if (avcodec_parameters_to_context(codecCtx, formatCtx->streams[videoStreamId]->codecpar) != 0)
	{
		assert(0);
	}

	auto cpus = pxl::platform::cpuCount();
	if (cpus > -1)
	{
		codecCtx->thread_count = cpus;
	}

	if (avcodec_open2(codecCtx, codec, nullptr) < 0)
	{
		assert(0);
	}

	buffer = nullptr;
	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, codecCtx->width, codecCtx->height, 32);
	buffer = (pxl::u8*)av_malloc(numBytes * sizeof(pxl::u8));
	av_image_fill_arrays(
		frameRgba->data,
		frameRgba->linesize,
		buffer,
		AV_PIX_FMT_RGBA,
		codecCtx->width,
		codecCtx->height,
		32
	);
}

void Decoder_ffmpeg::close()
{
	av_free(buffer);
	av_frame_free(&frameRgba);
	av_free(frameRgba);
	av_frame_free(&frame);
	av_free(frame);

	avcodec_close(codecCtx);
	avcodec_close(codecCtxOrig);
	avformat_close_input(&formatCtx);

	buffer = nullptr;
	frameRgba = nullptr;
	frame = nullptr;
	codecCtx = nullptr;
	codecCtxOrig = nullptr;
	formatCtx = nullptr;
}

pxl::Image Decoder_ffmpeg::currentFrameImage()
{
	struct SwsContext* sws_ctx = nullptr;
	sws_ctx = sws_getContext(
		codecCtx->width,
		codecCtx->height,
		codecCtx->pix_fmt,
		codecCtx->width,
		codecCtx->height,
		AV_PIX_FMT_RGBA,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);

	sws_scale(
		sws_ctx,
		(uint8_t const* const*)frame->data,
		frame->linesize,
		0,
		codecCtx->height,
		frameRgba->data,
		frameRgba->linesize
	);

	pxl::Image img(codecCtx->width, codecCtx->height);
	auto pxlPtr = img.pixels();
	const uint8_t* srcData[] = { frameRgba->data[0], frameRgba->data[1], frameRgba->data[2], frameRgba->data[3] };
	pxl::u8* dst[] = { &pxlPtr->r, nullptr, nullptr, nullptr };
	int linesizes[] = { img.width() * 4, 0,0,0 };
	av_image_copy(dst, linesizes, srcData, frameRgba->linesize, AV_PIX_FMT_RGBA, codecCtx->width, codecCtx->height);

	sws_freeContext(sws_ctx);
	return img;
}

pxl::i64 Decoder_ffmpeg::currentFrameNumber() const
{
	return currentFrame;
}

void Decoder_ffmpeg::flushDecoder()
{
	avcodec_flush_buffers(codecCtx);
	currentFrame = 0;
}

bool Decoder_ffmpeg::seek(int seekFrame)
{
	flushDecoder();
	pxl::i64 targetTime = dtsToTime[seekFrame];
	if (av_seek_frame(formatCtx, videoStreamId, targetTime, AVSEEK_FLAG_BACKWARD) < 0)
	{
		assert(0);
		return false;
	}
	seekTarget = targetTime;
	seekTargetFrame = seekFrame;
	return true;
}

pxl::video::FileInfo Decoder_ffmpeg::info() const
{
	pxl::video::FileInfo info;
	info.duration = formatCtx->duration / AV_TIME_BASE;
	info.frames = formatCtx->streams[videoStreamId]->nb_frames;
	info.framerate = av_q2d(formatCtx->streams[videoStreamId]->r_frame_rate);
	info.width = codecCtx->width;
	info.height = codecCtx->height;
	info.bitrate = codecCtx->bit_rate;
	info.gop_size = codecCtx->gop_size;
	info.file = file;
	return info;
}

bool Decoder_ffmpeg::decode()
{
	return decode(nullptr);
}

bool Decoder_ffmpeg::decode(FrameInfo* fillInfo)
{
	AVPacket packet;

	bool done = false;
	av_frame_unref(frame);
	while (av_read_frame(formatCtx, &packet) >= 0)
	{
		if (packet.stream_index == videoStreamId)
		{
			auto ret = avcodec_send_packet(codecCtx, &packet);
			if (ret < 0)
			{
				assert(0);
			}
			while (ret >= 0)
			{
				ret = avcodec_receive_frame(codecCtx, frame);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					// EOF exit loop
					break;
				}
				else if (ret < 0)
				{
					assert(0);
				}

				if (seekTarget != -1 && seekTarget != frame->pkt_dts) continue;
				
				if (seekTarget != -1)
				{
					seekTarget = -1;
					currentFrame = seekTargetFrame;
					seekTargetFrame = -1;
				}
				else
				{
					currentFrame++;
				}

				if (fillInfo != nullptr)
				{
					fillInfo->dts = frame->pkt_dts;
					fillInfo->pts = frame->pts;
				}
				done = true;
				break;
			}
		}

		av_packet_unref(&packet);

		if (done)
		{
			break;
		}
	}

	return done;
}

void Decoder_ffmpeg::calculateFrames()
{
	FrameInfo info;
	info.pts = 0;
	info.dts = 0;
	info.frameNumber = 0;

	while (decode(&info))
	{
		info.frameNumber++;
		dtsToTime[info.frameNumber] = info.dts;
	}

	seek(1);
}

pxl::video::DecoderRef pxl::video::createDecoder()
{
	return std::make_shared<Decoder_ffmpeg>();
}

#endif