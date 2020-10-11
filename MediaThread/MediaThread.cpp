#include "MediaThread.h"
#include <thread>

int MediaThread::Start()
{
	if (t == nullptr) {
		stopFlag = 0;
		t = new std::thread(&MediaThread::run, this);
	}

	return 0;
}

int MediaThread::Stop()
{
	if (t != nullptr) {
		stopFlag = 1;
		t->join();
		delete t;
		t = nullptr;
	}
	return 0;
}