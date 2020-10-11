#pragma once

extern "C"
{
#include <libavformat/avformat.h>
}

class MediaAVFramePrivate
{
public:
	AVFrame* frame = nullptr;
	double ptsSec = 0.0;
};