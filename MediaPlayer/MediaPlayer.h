#pragma once

#include <string>
#include "MediaThread/MediaThread.h"
#include "MediaAV/MediaAV.h"
#include "MediaQueue/MediaQueue.h"

enum MediaDecoderType
{
	MediaDECODER_TYPE_VIDEO = 0,
	MediaDECODER_TYPE_AUDIO = 1
};

enum MediaPlayerCtrStatus
{
	MediaPLAYER_CTR_STATUS_PLAYING = 0,
	MediaPLAYER_CTR_STATUS_PAUSEING = 1
};

class MediaPlayerCtr : public MediaThread
{
public:
	MediaPlayerCtr(double seekTime = 0.0);
	~MediaPlayerCtr();

	virtual void run();

	int Play();
	int Pause();

	int GetVideoQueueSize();
	int GetAudioQueueSize();

	int PushFrameToVideoQueue(MediaAVFrame * frame);
	int PushFrameToAudioQueue(MediaAVFrame* frame);
private:
	MediaQueue<MediaAVFrame> videoQueue;
	MediaQueue<MediaAVFrame> audioQueue;

	MediaPlayerCtrStatus status = MediaPlayerCtrStatus::MediaPLAYER_CTR_STATUS_PLAYING;

	double seekTime = 0.0;
};

class MediaPlayerReaderThread : public MediaThread
{
public:
	MediaPlayerReaderThread(std::string path, double seekTime, MediaPlayerCtr * playerCtr);
	~MediaPlayerReaderThread();

	virtual void run();

private:
	std::string path;
	MediaPlayerCtr* playerCtr = nullptr;
	double seekTime = 0.0;
};

class MediaPlayerDecoderThread : public MediaThread
{
public:
	MediaPlayerDecoderThread(MediaPlayerCtr* playerCtr, MediaDecoderType type);
	~MediaPlayerDecoderThread();

	virtual void run();

	int Init(MediaAVStream * avStream);

	int PutPacket(MediaAVPacket * pkt);
	int GetPacketQueueSize();

private:
	MediaAVDecoder* decoder = nullptr;
	MediaQueue<MediaAVPacket> packetQueue;

	MediaPlayerCtr* playerCtr = nullptr;

	MediaDecoderType type;
};

class MediaPlayer
{
public:
	MediaPlayer(std::string path);
	~MediaPlayer();

	int Open(double time = 0.0);
	int Stop();

	int Play();
	int Pause();

	int Seek(double time);

private:
	std::string path;
	MediaPlayerReaderThread* readerThread = nullptr;

	MediaPlayerCtr* playerCtr = nullptr;
};