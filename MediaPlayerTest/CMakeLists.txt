cmake_minimum_required (VERSION 3.8)

include_directories(../)

link_directories(../3rdpart/ffmpeg/lib)

add_executable (
	MediaPlayerTest
	Main.cpp
)

target_link_libraries(MediaPlayerTest MediaThread)
target_link_libraries(MediaPlayerTest MediaAV)
target_link_libraries(MediaPlayerTest MediaPlayer)

target_link_libraries(MediaPlayerTest avcodec)
target_link_libraries(MediaPlayerTest avdevice)
target_link_libraries(MediaPlayerTest avformat)
target_link_libraries(MediaPlayerTest avutil)
target_link_libraries(MediaPlayerTest swresample)
target_link_libraries(MediaPlayerTest swscale)