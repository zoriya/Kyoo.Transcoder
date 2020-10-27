cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 11)

set (PROJECT_VERSION "1.0")
project(transcoder VERSION ${PROJECT_VERSION})

include_directories(ffmpeg)
include_directories(include)

add_library(transcoder SHARED
        src/helper.c
        src/info.c
        src/transmuxer.c
        src/path_helper.c
        src/destroyer.c
        src/subtitles.c)
set_property(TARGET transcoder PROPERTY C_STANDARD 11)

target_link_libraries(transcoder
        ${CMAKE_SOURCE_DIR}/ffmpeg/libavformat/libavformat.a
        ${CMAKE_SOURCE_DIR}/ffmpeg/libavcodec/libavcodec.a
        ${CMAKE_SOURCE_DIR}/ffmpeg/libavutil/libavutil.a
        ${CMAKE_SOURCE_DIR}/ffmpeg/libavdevice/libavdevice.a
        ${CMAKE_SOURCE_DIR}/ffmpeg/libswresample/libswresample.a)

if(WIN32)
    target_link_libraries(transcoder wsock32 ws2_32)
    target_link_libraries(transcoder Secur32 Secur32)
    target_link_libraries(transcoder Bcrypt Bcrypt)
endif()

include(ExternalProject)
ExternalProject_Add(FFMPEG
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/ffmpeg
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./configure --pkg-config-flags=--static --disable-shared --enable-static --disable-zlib --disable-iconv --disable-asm --disable-ffplay --disable-ffprobe
        BUILD_COMMAND make
        INSTALL_COMMAND cmake -E echo "Skipping install step."
)
