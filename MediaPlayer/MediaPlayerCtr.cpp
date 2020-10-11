#include "MediaPlayer.h"

MediaPlayerCtr::MediaPlayerCtr(double _seekTime)
{
	seekTime = _seekTime;
}

MediaPlayerCtr::~MediaPlayerCtr()
{

}

int MediaPlayerCtr::Play()
{
	status = MediaPlayerCtrStatus::MediaPLAYER_CTR_STATUS_PLAYING;
	return 0;
}

int MediaPlayerCtr::Pause()
{
	status = MediaPlayerCtrStatus::MediaPLAYER_CTR_STATUS_PAUSEING;
	return 0;
}

void MediaPlayerCtr::run()
{
	MediaPlayerReaderThread readerThread("c:/DemoVideo/demo_video.mp4", seekTime, this);
	readerThread.Start();

	// ��ȡ�߳�����ʱ���ʱ�� start_time
	long long startTime = MediaAVTime::GetTime();

	MediaAVFrame* videoFrame = nullptr;
	MediaAVFrame* audioFrame = nullptr;

	long long sleepCountTime = 0;

	while (!stopFlag) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		long long sleepTimeStart = MediaAVTime::GetTime();
		while (status == MediaPlayerCtrStatus::MediaPLAYER_CTR_STATUS_PAUSEING) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		long long sleepTimeEnd = MediaAVTime::GetTime();
		long long sleepDTime = sleepTimeEnd - sleepTimeStart;
		sleepCountTime += sleepDTime;

		// ��ȡ��ǰ��ʱ�� now_time
		long long nowTime = MediaAVTime::GetTime();

		// ��ȡ����ǰʱ��Ϳ�ʼʱ��Ĳ�ֵ d_time
		long long dTime = nowTime - startTime;
		dTime = dTime - sleepCountTime;

		dTime = dTime + (long long)(seekTime * 1000);

		// printf("DTime: %lld\n", dTime);

		// ����Ƶ��������У���ȡһ֡��Ƶ frame_pts 
		if (videoFrame == nullptr) {
			// ����ȡһ֡����
			videoQueue.Pop(&videoFrame);
		}

		// printf("Video Queue Size: %d\n", videoQueue.Size());

		if (videoFrame != nullptr) {
			if (videoFrame->GetPts() < (long long)(seekTime * 1000)) {
				delete videoFrame;
				videoFrame = nullptr;
			}
		}
		
		if (videoFrame != nullptr) {
			// ��� frame_pts <= d_time
			if (videoFrame->GetPts() <= dTime) {
				// ��֡��Ƶ��Ӧ���������ų���
				printf("Video Frame: %lld\n", videoFrame->GetPts());
				delete videoFrame;
				videoFrame = nullptr;
			}
			// ����
				// ��֡��Ƶ���������ŵ�ʱ��,�����������������ȥ������Ƶ
			else {

			}
		}
		



		// ����Ƶ��������У���ȡһ֡��Ƶ frame_pts 
		if (audioFrame == nullptr) {
			audioQueue.Pop(&audioFrame);
		}

		if (audioFrame != nullptr) {
			if (audioFrame->GetPts() < (long long)(seekTime * 1000)) {
				delete audioFrame;
				audioFrame = nullptr;
			}
		}

		if (audioFrame != nullptr) {
			// ��� frame_pts <= d_time
			if (audioFrame->GetPts() <= dTime) {
				// ��֡��Ƶ��Ӧ���������ų���
				// printf("Audio Frame\n");
				delete audioFrame;
				audioFrame = nullptr;
			}
			else {
				// ��֡��Ƶ���������ŵ�ʱ��,�������������
			}
		}
	}

	readerThread.Stop();
}

int MediaPlayerCtr::GetVideoQueueSize()
{
	return videoQueue.Size();
}

int MediaPlayerCtr::GetAudioQueueSize()
{
	return audioQueue.Size();
}

int MediaPlayerCtr::PushFrameToVideoQueue(MediaAVFrame* frame)
{
	// printf("Vudio Queue Size: %d\n", videoQueue.Size());
	return videoQueue.Push(frame);
}

int MediaPlayerCtr::PushFrameToAudioQueue(MediaAVFrame* frame)
{
	// printf("Audio Queue Size: %d\n", audioQueue.Size());
	return audioQueue.Push(frame);
}