# CMakeList.txt: MediaPlayer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
cmake_minimum_required (VERSION 3.8)

include_directories(../)

link_directories(../3rdpart/ffmpeg/lib)

# 将源代码添加到此项目的可执行文件。
add_library (
	MediaPlayer
	MediaPlayer.cpp
	MediaPlayer.h
	MediaPlayerReaderThread.cpp
	MediaPlayerDecoderThread.cpp
	MediaPlayerCtr.cpp
)

target_link_libraries(MediaPlayer MediaThread)
target_link_libraries(MediaPlayer MediaAV)

target_link_libraries(MediaPlayer avcodec)
target_link_libraries(MediaPlayer avdevice)
target_link_libraries(MediaPlayer avformat)
target_link_libraries(MediaPlayer avutil)
target_link_libraries(MediaPlayer swresample)
target_link_libraries(MediaPlayer swscale)