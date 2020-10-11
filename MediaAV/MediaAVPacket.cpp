#include "MediaAV.h"

#include "MediaAVPacketPrivate.h"

MediaAVPacket::MediaAVPacket()
{
	imp = new MediaAVPacketPrivate();
	imp->pkt = av_packet_alloc();
}

MediaAVPacket::~MediaAVPacket()
{
	if (imp->pkt != nullptr) {
		av_packet_free(&imp->pkt);
		imp->pkt = nullptr;
	}

	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}

int MediaAVPacket::GetIndex()
{
	return imp->pkt->stream_index;
}