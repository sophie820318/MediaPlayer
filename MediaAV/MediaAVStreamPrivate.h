#pragma once

extern "C"
{
#include <libavformat/avformat.h>
}

class MediaAVStreamPrivate
{
public:
	AVCodecParameters* codecpar = nullptr;
};