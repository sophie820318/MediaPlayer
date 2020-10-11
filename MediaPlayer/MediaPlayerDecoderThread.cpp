#include "MediaPlayer.h"

MediaPlayerDecoderThread::MediaPlayerDecoderThread(MediaPlayerCtr* _playerCtr, MediaDecoderType _type)
{
	decoder = new MediaAVDecoder();
	playerCtr = _playerCtr;
	type = _type;
}

MediaPlayerDecoderThread::~MediaPlayerDecoderThread()
{
	if (decoder != nullptr) {
		delete decoder;
		decoder = nullptr;
	}
}

int MediaPlayerDecoderThread::Init(MediaAVStream* avStream)
{
	return decoder->Init(avStream);
}

int MediaPlayerDecoderThread::PutPacket(MediaAVPacket* pkt)
{
	return packetQueue.Push(pkt);
}

int MediaPlayerDecoderThread::GetPacketQueueSize()
{
	return packetQueue.Size();
}

void MediaPlayerDecoderThread::run()
{
	int frameCount = 0;
	// ���н������
	while (!stopFlag) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (type == MediaDecoderType::MediaDECODER_TYPE_VIDEO) {
			if (playerCtr->GetVideoQueueSize() > 10) {
				continue;
			}
		}
		else if (type == MediaDecoderType::MediaDECODER_TYPE_AUDIO) {
			if (playerCtr->GetAudioQueueSize() > 20) {
				continue;
			}
		}



		MediaAVPacket* pkt = nullptr;
		int ret = packetQueue.Pop(&pkt);
		if (ret) {
			// ����Ϊ�գ�ȡ���� Packet
			continue;
		}

		decoder->SendPacket(pkt);
		while (1) {
			MediaAVFrame* frame = new MediaAVFrame();
			ret = decoder->RecvFrame(frame);
			if (ret) {
				break;
			}
			frameCount++;
			// printf("Decoder Success: %d\n", frameCount);
			// std::this_thread::sleep_for(std::chrono::seconds(1));

			// ͨ�� playerCtr �ľ������������ Frame ����
			if (type == MediaDecoderType::MediaDECODER_TYPE_VIDEO) {
				playerCtr->PushFrameToVideoQueue(frame);
			}
			else if (type == MediaDecoderType::MediaDECODER_TYPE_AUDIO) {
				playerCtr->PushFrameToAudioQueue(frame);
			}
		}
	}
}