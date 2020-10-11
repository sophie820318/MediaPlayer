#include "MediaAV.h"

#include "MediaAVStreamPrivate.h"

MediaAVStream::MediaAVStream()
{
	imp = new MediaAVStreamPrivate();
	imp->codecpar = avcodec_parameters_alloc();
}

MediaAVStream::~MediaAVStream()
{
	if (imp->codecpar != nullptr) {
		avcodec_parameters_free(&imp->codecpar);
		imp->codecpar = nullptr;
	}
	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}