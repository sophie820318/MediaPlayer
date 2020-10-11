#pragma once

class MediaAVStreamPrivate;
class MediaAVStream;

class MediaAVPacketPrivate;

class MediaAVPacket
{
public:
	MediaAVPacket();
	~MediaAVPacket();

	int GetIndex();
public:
	MediaAVPacketPrivate* imp = nullptr;
};

class MediaAVFramePrivate;

class MediaAVFrame
{
public:
	MediaAVFrame();
	~MediaAVFrame();

	int VideoPrint();
	int AudioPrint();

	int GetW();
	int GetH();

	int GetY(unsigned char* y);
	int GetU(unsigned char* u);
	int GetV(unsigned char* v);

	long long GetPts();

public:
	MediaAVFramePrivate* imp = nullptr;
};



class MediaAVReaderPrivate;

class MediaAVReader
{
public:
	MediaAVReader();
	~MediaAVReader();

	int Open(const char * path);

	int GetStreamCount();
	int GetStream(MediaAVStream * stream, int streamId);

	int GetVideoStreamIndex();
	int GetAudioStreamIndex();

	int Close();

	int Read(MediaAVPacket * packet);

	int Seek(double time);
private:
	MediaAVReaderPrivate* imp = nullptr;
};

class MediaAVStreamPrivate;

class MediaAVStream
{
public:
	MediaAVStream();
	~MediaAVStream();
public:
	int streamIndex = -1;
	int timebaseNum = 0;
	int timebaseDen = 0;
	MediaAVStreamPrivate* imp = nullptr;
};


class MediaAVDecoderPrivate;

class MediaAVDecoder
{
public:
	MediaAVDecoder();
	~MediaAVDecoder();

	int Init(MediaAVStream * stream);

	int SendPacket(MediaAVPacket * pkt);

	int RecvFrame(MediaAVFrame * frame);

	int Close();

private:
	MediaAVDecoderPrivate* imp = nullptr;

	int timebaseNum = 0;
	int timebaseDen = 0;
};


class MediaAVTime
{
public:
	static long long GetTime();
};