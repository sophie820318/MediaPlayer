#pragma once

extern "C"
{
#include <libavformat/avformat.h>
}

class MediaAVPacketPrivate
{
public:
	AVPacket * pkt = nullptr;
};