cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 11)

set(PROJECT_VERSION "1.0")
project(transcoder VERSION ${PROJECT_VERSION})

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

target_link_libraries(transcoder
	${CMAKE_SOURCE_DIR}/ffmpeg/libavformat/libavformat.a
	${CMAKE_SOURCE_DIR}/ffmpeg/libavcodec/libavcodec.a
	${CMAKE_SOURCE_DIR}/ffmpeg/libavutil/libavutil.a
)
target_link_libraries(transcoder m)

if(WIN32)
	target_link_libraries(transcoder wsock32 ws2_32)
	target_link_libraries(transcoder Secur32 Secur32)
	target_link_libraries(transcoder Bcrypt Bcrypt)
endif()

include(ExternalProject)
ExternalProject_Add(FFMPEG
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/ffmpeg
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ./configure --pkg-config-flags=--static --disable-shared --enable-static
       		--disable-zlib --disable-iconv --disable-asm
		--disable-programs --disable-doc
		--disable-avdevice --disable-swresample --disable-swscale --disable-postproc --disable-network
	BUILD_COMMAND make
	INSTALL_COMMAND cmake -E echo "Skipping install step."
)
add_dependencies(transcoder FFMPEG)
