cmake_minimum_required(VERSION 3.12)
set(CMAKE_C_STANDARD 11)

set(PROJECT_VERSION "1.0")
project(transcoder VERSION ${PROJECT_VERSION} LANGUAGES C)

include_directories(ffmpeg)
include_directories(include)

add_library(transcoder SHARED 
	src/helper.c
	src/info.c
	src/extractor.c
	src/transmuxer.c
	src/path_helper.c
	src/destroyer.c)
set_property(TARGET transcoder PROPERTY C_STANDARD 11)

target_link_libraries(transcoder avformat avcodec avutil)
target_link_directories(transcoder PRIVATE
	${CMAKE_SOURCE_DIR}/ffmpeg/libavformat
	${CMAKE_SOURCE_DIR}/ffmpeg/libavcodec
	${CMAKE_SOURCE_DIR}/ffmpeg/libavutil
)
target_link_libraries(transcoder m pthread)

if(WIN32)
	target_link_libraries(transcoder wsock32 ws2_32)
	target_link_libraries(transcoder Secur32 Secur32)
	target_link_libraries(transcoder Bcrypt Bcrypt)
endif()

include(ExternalProject)
ExternalProject_Add(FFMPEG
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/ffmpeg
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ./configure --disable-shared --enable-static --enable-pic
            --disable-debug --enable-gpl --enable-version3 --disable-asm
		    --disable-programs --disable-doc --disable-autodetect
		    --disable-avdevice --disable-swresample --disable-swscale --disable-postproc --disable-network
	BUILD_COMMAND make
	INSTALL_COMMAND cmake -E echo "Skipping install step."
)
add_dependencies(transcoder FFMPEG)
set_property(TARGET transcoder PROPERTY POSITION_INDEPENDENT_CODE ON)

