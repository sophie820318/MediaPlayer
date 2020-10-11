#include "MediaAV.h"

#include "MediaAVDecoderPrivate.h"
#include "MediaAVStreamPrivate.h"
#include "MediaAVPacketPrivate.h"
#include "MediaAVFramePrivate.h"

MediaAVDecoder::MediaAVDecoder()
{
	imp = new MediaAVDecoderPrivate();
	imp->codecContext = avcodec_alloc_context3(nullptr);
}

MediaAVDecoder::~MediaAVDecoder()
{
	if (imp->codecContext != nullptr) {
		avcodec_free_context(&imp->codecContext);
		imp->codecContext = nullptr;
	}

	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}

int MediaAVDecoder::Init(MediaAVStream* stream)
{
	timebaseNum = stream->timebaseNum;
	timebaseDen = stream->timebaseDen;

	avcodec_parameters_to_context(imp->codecContext, stream->imp->codecpar);
	AVCodec * avCodec = avcodec_find_decoder(imp->codecContext->codec_id);

	int ret = avcodec_open2(imp->codecContext, avCodec, nullptr);
	if (ret) {
		printf("avcodec_open2 fail ret:%d\n", ret);
		return -1;
	}

	return 0;
}


int MediaAVDecoder::SendPacket(MediaAVPacket* pkt)
{
	int ret = 0;
	if (pkt == nullptr) {
		ret = avcodec_send_packet(imp->codecContext, nullptr);
	}
	else {
		ret = avcodec_send_packet(imp->codecContext, pkt->imp->pkt);
	}
	
	return ret;
}

int MediaAVDecoder::RecvFrame(MediaAVFrame* frame)
{
	int ret = avcodec_receive_frame(imp->codecContext, frame->imp->frame);
	if (!ret) {
		// 将秒级时间戳计算并且赋值
		frame->imp->ptsSec = frame->imp->frame->pts * 1.0 * timebaseNum / timebaseDen;
	}
	return ret;
}

int MediaAVDecoder::Close()
{
	return avcodec_close(imp->codecContext);
}