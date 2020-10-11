#pragma once

extern "C"
{
#include <libavformat/avformat.h>
}

class MediaAVDecoderPrivate
{
public:
	AVCodecContext * codecContext = nullptr;
};