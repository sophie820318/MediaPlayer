#include "MediaPlayer.h"

MediaPlayer::MediaPlayer(std::string _path)
{
	path = _path;
}

MediaPlayer::~MediaPlayer()
{

}

int MediaPlayer::Open(double time)
{
	/*
	if (readerThread == nullptr) {
		readerThread = new MediaPlayerReaderThread(path);
		readerThread->Start();

		return 0;
	}
	*/

	if (playerCtr == nullptr) {
		playerCtr = new MediaPlayerCtr(time);
		playerCtr->Start();
		return 0;
	}
	
	return -1;
}

int MediaPlayer::Stop()
{
	/*
	if (readerThread != nullptr) {
		readerThread->Stop();
		delete readerThread;
		readerThread = nullptr;
	}
	*/
	if (playerCtr != nullptr) {
		playerCtr->Stop();
		delete playerCtr;
		playerCtr = nullptr;
	}

	return 0;
}

int MediaPlayer::Play()
{
	if (playerCtr != nullptr) {
		playerCtr->Play();
	}
	return 0;
}

int MediaPlayer::Pause()
{
	if (playerCtr != nullptr) {
		playerCtr->Pause();
	}
	return 0;
}

int MediaPlayer::Seek(double time)
{
	Stop();
	Open(time);
	return 0;
}