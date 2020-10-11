#include "MediaAV.h"

#include "MediaAVReaderPrivate.h"
#include "MediaAVPacketPrivate.h"
#include "MediaAVDecoderPrivate.h"
#include "MediaAVStreamPrivate.h"

MediaAVReader::MediaAVReader()
{
	imp = new MediaAVReaderPrivate();
	imp->formatCtx = avformat_alloc_context();
}

MediaAVReader::~MediaAVReader()
{
	if (imp->formatCtx != nullptr) {
		avformat_free_context(imp->formatCtx);
		imp->formatCtx = nullptr;
	}

	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}

int MediaAVReader::Open(const char* path)
{
	if (imp->formatCtx == nullptr) {
		return -1;
	}
	int ret = avformat_open_input(&imp->formatCtx, path, nullptr, nullptr);

	if (!ret) {
		avformat_find_stream_info(imp->formatCtx, nullptr);
	}

	return ret;
}

int MediaAVReader::GetStreamCount()
{
	return imp->formatCtx->nb_streams;
}

int MediaAVReader::GetStream(MediaAVStream* avStream, int streamId)
{
	AVStream* ffmpegStream = imp->formatCtx->streams[streamId];

	// ffmpegStream->time_base

	avStream->timebaseNum = ffmpegStream->time_base.num;
	avStream->timebaseDen = ffmpegStream->time_base.den;
	avStream->streamIndex = ffmpegStream->index;

	avcodec_parameters_copy(avStream->imp->codecpar, ffmpegStream->codecpar);
	
	return 0;
}

int MediaAVReader::GetVideoStreamIndex()
{
	return av_find_best_stream(imp->formatCtx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, NULL, NULL);
}

int MediaAVReader::GetAudioStreamIndex()
{
	return av_find_best_stream(imp->formatCtx, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, NULL, NULL);
}

int MediaAVReader::Close()
{
	if (imp->formatCtx == nullptr) {
		return -1;
	}
	avformat_close_input(&imp->formatCtx);
	return 0;
}

int MediaAVReader::Read(MediaAVPacket* packet)
{
	if (imp->formatCtx == nullptr) {
		return -1;
	}
	int ret = av_read_frame(imp->formatCtx, packet->imp->pkt);

	return ret;
}

int MediaAVReader::Seek(double time)
{
	if (imp->formatCtx == nullptr) {
		return -1;
	}

	int64_t timestamp = (int64_t)(time * AV_TIME_BASE);
	av_seek_frame(imp->formatCtx, -1, timestamp, AVSEEK_FLAG_BACKWARD);
	return 0;
}