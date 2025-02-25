#ifdef PXL_VIDEO_FFMPEG

#include <pxl/backends/video_backend.h>
#include <pxl/backends/platform_backend.h>
#include <pxl/containers/map.h>
#include <pxl/math/calc.h>
#include <pxl/log.h>

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

	struct FrameInfo
	{
		pxl::i64 packetDts;
		pxl::i64 packetPts;
		pxl::i64 pts;
		pxl::i64 dts;
		bool key;
		int frameNumber;
	};

	AVCodecID pxlToFFmpeg(pxl::video::Codec codec)
	{
		switch (codec) {
		case pxl::video::Codec::MPEG4: return AV_CODEC_ID_MPEG4;
		case pxl::video::Codec::H264: return AV_CODEC_ID_H264;
		}

		assert(0);
		return AV_CODEC_ID_MPEG4;
	}

	pxl::video::Codec ffmpegToPxl(AVCodecID codec)
	{
		if (codec == AV_CODEC_ID_MPEG4) return pxl::video::Codec::MPEG4;
		else if (codec == AV_CODEC_ID_H264) return pxl::video::Codec::H264;

		assert(0);
		return pxl::video::Codec::MPEG4;
	}

	void initFFMpeg()
	{
		if (!s_ffmpeg_inited)
		{
			av_register_all();
			avcodec_register_all();
			av_log_set_level(AV_LOG_QUIET);
			s_ffmpeg_inited = true;
		}
	}
	bool isEncoderSupported(pxl::video::Codec codec)
	{
		auto c = avcodec_find_encoder(pxlToFFmpeg(codec));
		return c != nullptr;
	}
}

class Frame_ffmpeg : public pxl::video::Frame
{
public:
	Frame_ffmpeg(const pxl::Image& image, pxl::i64 frame) : image(image), frameNum(frame)
	{
	}
	Frame_ffmpeg(pxl::Image&& image, pxl::i64 frame) : image(image), frameNum(frame)
	{
	}
	~Frame_ffmpeg()
	{
	}
	const pxl::Image& rgba() const override
	{
		return image;
	}
	pxl::i64 frame() const override
	{
		return frameNum;
	}

	pxl::Image image;
	pxl::i64 frameNum;
};

class Encoder_ffmpeg : public pxl::video::Encoder
{
public:
	Encoder_ffmpeg(const pxl::video::EncoderInfo& info);
	~Encoder_ffmpeg();
	bool isOk() const override;
	void add(const pxl::video::FrameRef& image) override;
	void save() override;
private:
	bool m_ok = false;
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

Encoder_ffmpeg::Encoder_ffmpeg(const pxl::video::EncoderInfo& info) : info(info) {

	codecCtx = nullptr;
	frame = nullptr;
	frameRgba = nullptr;

	frame = av_frame_alloc();
	frameRgba = av_frame_alloc();
	frameYuv = av_frame_alloc();
	buffer = nullptr;

	//
	format = av_guess_format(nullptr, info.file.data(), nullptr);
	if (format == nullptr)
	{
		pxl::log::error("[video encoder] Could not guess format");
		assert(0);
		return;
	}
	if (avformat_alloc_output_context2(&formatContext, format, nullptr, info.file.data()) < 0)
	{
		pxl::log::error("[video encoder] Could not allocate context");
		assert(0);
		return;
	}


	AVCodec* codec = avcodec_find_encoder(pxlToFFmpeg(info.codec));
	if (!codec)
	{
		pxl::log::error("[video encoder] Could not find encoder");
		assert(0);
		return;
	}

	stream = avformat_new_stream(formatContext, codec);

	if (!stream)
	{
		pxl::log::error("[video encoder] Could not open stream");
		assert(0);
		return;
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
	codecCtx->time_base = av_inv_q(av_d2q(info.framerate, 100));

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
		codecCtx->gop_size = info.gop_size;
	}
	else if (stream->codecpar->codec_id == AV_CODEC_ID_MPEG4)
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
		pxl::log::error("[video encoder] Could not open video codec");
		assert(0);
		return;
	}

	if (!(format->flags & AVFMT_NOFILE))
	{
		if ((avio_open(&formatContext->pb, info.file.data(), AVIO_FLAG_WRITE)) < 0)
		{
			pxl::log::error("[video encoder] Could not open output context");
			assert(0);
			return;
		}
	}

	if (avformat_write_header(formatContext, nullptr) < 0)
	{
		pxl::log::error("[video encoder] Could not write header");
		assert(0);
		return;
	}

	pts = 0;

	frameYuv->width = codecCtx->width;
	frameYuv->height = codecCtx->height;
	frameYuv->format = codecCtx->pix_fmt;

	if (av_frame_get_buffer(frameYuv, 32) < 0)
	{
		assert(0);
	}

	m_ok = true;
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

void Encoder_ffmpeg::add(const pxl::video::FrameRef& frame)
{
	auto yuvFrame = getYuv(frame->rgba());

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

		if (codecCtx->coded_frame->key_frame) {
			packet.flags |= AV_PKT_FLAG_KEY;
		}

		av_interleaved_write_frame(formatContext, &packet);
	}
	av_packet_unref(&packet);
	pts++;
}

bool Encoder_ffmpeg::isOk() const
{
	return m_ok;
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

pxl::video::EncoderRef pxl::video::createEncoder(const EncoderInfo& info)
{
	initFFMpeg();
	if (isEncoderSupported(info.codec))
	{
		return std::make_shared<Encoder_ffmpeg>(info);
	}
	else
	{
		return nullptr;
	}
}

class Decoder_ffmpeg : public pxl::video::Decoder
{
public:
	Decoder_ffmpeg();
	~Decoder_ffmpeg();
	void open(const pxl::String& file) override;
	void close() override;
	void calculateFrames(pxl::video::Progress& progress) override;
	pxl::video::FrameRef currentFrame() override;
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
	pxl::i64 currentFrameNum;
	FrameInfo currentFrameInfo;
	pxl::Map<pxl::i64, pxl::i64> frameToDts;
	pxl::String file;
};

Decoder_ffmpeg::Decoder_ffmpeg() : seekTarget(-1), seekTargetFrame(0),
formatCtx(nullptr), codecCtxOrig(nullptr), codecCtx(nullptr), frame(nullptr), frameRgba(nullptr), buffer(nullptr), videoStreamId(-1), currentFrameNum(0) {
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

	if (avformat_open_input(&formatCtx, file.data(), nullptr, nullptr) < 0)
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
			videoStreamIds.push_back(i);
			break;
		}
		else if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStreamIds.push_back(i);
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

pxl::video::FrameRef Decoder_ffmpeg::currentFrame()
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

	auto ffmpegFrame = new Frame_ffmpeg(std::move(img), currentFrameNum);

	return  pxl::video::FrameRef(ffmpegFrame);
}

pxl::i64 Decoder_ffmpeg::currentFrameNumber() const
{
	return currentFrameNum;
}

void Decoder_ffmpeg::flushDecoder()
{
	//flush codec
	auto ret = avcodec_send_packet(codecCtx, nullptr);
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
	}
	avcodec_flush_buffers(codecCtx);
	currentFrameNum = 0;
}

bool Decoder_ffmpeg::seek(int seekFrame)
{
	if (seekFrame < 1) return false;

	auto fileInfo = info();

	if (seekFrame > fileInfo.frames)
	{
		return false;
	}

	flushDecoder();
	pxl::i64 targetTime = frameToDts[pxl::calc::max(seekFrame - (codecCtx->delay + codecCtx->gop_size + 1), 0)];


	if (av_seek_frame(formatCtx, videoStreamId, targetTime, AVSEEK_FLAG_BACKWARD) < 0)
	{
		assert(0);
		return false;
	}
	seekTarget = frameToDts[seekFrame];
	seekTargetFrame = seekFrame;
	return true;
}

pxl::video::FileInfo Decoder_ffmpeg::info() const
{
	pxl::video::FileInfo info;
	info.duration = formatCtx->duration / AV_TIME_BASE;
	info.frames = formatCtx->streams[videoStreamId]->nb_frames;
	if (!frameToDts.empty())
	{
		info.frames = frameToDts.size() - 1U;
	}
	info.framerate = av_q2d(formatCtx->streams[videoStreamId]->r_frame_rate);
	info.width = codecCtx->width;
	info.height = codecCtx->height;
	info.bitrate = codecCtx->bit_rate;
	info.gop_size = codecCtx->gop_size;
	info.file = file;
	info.codec = ffmpegToPxl(formatCtx->streams[videoStreamId]->codecpar->codec_id);
	info.seekToPts = formatCtx->iformat->flags & AVFMT_SEEK_TO_PTS;
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
					currentFrameNum = seekTargetFrame;
					seekTargetFrame = -1;
				}
				else
				{
					currentFrameNum++;
				}

				if (fillInfo != nullptr)
				{
					fillInfo->dts = frame->pkt_dts;
					fillInfo->pts = frame->pts;
					fillInfo->packetDts = packet.dts;
					fillInfo->packetPts = packet.pts;
					fillInfo->key = frame->key_frame;
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

void Decoder_ffmpeg::calculateFrames(pxl::video::Progress& progress)
{
	progress.max = formatCtx->streams[videoStreamId]->nb_frames;
	FrameInfo info;
	info.pts = 0;
	info.dts = 0;
	info.packetDts = 0;
	info.packetPts = 0;
	info.frameNumber = 0;
	frameToDts[0] = 0;
	while (decode(&info))
	{
		info.frameNumber++;
		frameToDts[info.frameNumber] = info.dts;
		progress.current = info.frameNumber;
		if (progress.update != nullptr)
		{
			progress.update();
		}
	}

	flushDecoder();
}



pxl::video::DecoderRef pxl::video::createDecoder()
{
	initFFMpeg();
	return std::make_shared<Decoder_ffmpeg>();
}

pxl::video::FrameRef pxl::video::createFrame(const pxl::Image& image, pxl::i64 frame)
{
	return std::make_shared<Frame_ffmpeg>(image, frame);
}

#endif

