#pragma once

extern "C"
{
#include <libavformat/avformat.h>
}

class MediaAVReaderPrivate
{
public:
	AVFormatContext* formatCtx = nullptr;
};