#include "MediaPlayer.h"
#include "MediaAV/MediaAV.h"

MediaPlayerReaderThread::MediaPlayerReaderThread(std::string _path, double _seekTime, MediaPlayerCtr* _playerCtr)
{
	path = _path;
	playerCtr = _playerCtr;
	seekTime = _seekTime;
}

MediaPlayerReaderThread::~MediaPlayerReaderThread()
{

}

void MediaPlayerReaderThread::run()
{
	MediaAVReader reader;
	int ret = reader.Open(path.c_str());
	if (ret) {
		return;
	}

	reader.Seek(seekTime);

	int videoStreamIndex = reader.GetVideoStreamIndex();
	int audioStreamIndex = reader.GetAudioStreamIndex();

	// TODO 初始化解码器
	MediaPlayerDecoderThread* videoDecoderThread = new MediaPlayerDecoderThread(playerCtr, MediaDecoderType::MediaDECODER_TYPE_VIDEO);
	MediaPlayerDecoderThread* audioDecoderThread = new MediaPlayerDecoderThread(playerCtr, MediaDecoderType::MediaDECODER_TYPE_AUDIO);

	MediaAVStream videoStream;
	reader.GetStream(&videoStream, videoStreamIndex);
	videoDecoderThread->Init(&videoStream);

	MediaAVStream audioStream;
	reader.GetStream(&audioStream, audioStreamIndex);
	audioDecoderThread->Init(&audioStream);

	videoDecoderThread->Start();
	audioDecoderThread->Start();

	while (!stopFlag) {
		if (videoDecoderThread->GetPacketQueueSize() > 5 && audioDecoderThread->GetPacketQueueSize() > 5) {
			continue;
		}

		MediaAVPacket* pkt = new MediaAVPacket();
		int ret = reader.Read(pkt);
		if (ret) {
			delete pkt;
			pkt = nullptr;
			break;
		}

		if (pkt->GetIndex() == videoStreamIndex) {
			videoDecoderThread->PutPacket(pkt);
		}

		if (pkt->GetIndex() == audioStreamIndex) {
			audioDecoderThread->PutPacket(pkt);
		}

		// 将 Packet 放入缓存
		// printf("Read Packet Success\n");

		// delete pkt;
		// pkt = nullptr;
	}

	videoDecoderThread->Stop();
	audioDecoderThread->Stop();

	reader.Close();
}