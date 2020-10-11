cmake_minimum_required (VERSION 3.8)

include_directories(../3rdpart/ffmpeg/include)

add_library(
	MediaAV
	MediaAVReader.cpp
	MediaAVPacket.cpp
	MediaAVDecoder.cpp
	MediaAVStream.cpp
	MediaAVFrame.cpp
	MediaAVTime.cpp
)