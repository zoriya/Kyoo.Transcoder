cmake_minimum_required(VERSION 3.15)

set (PROJECT_VERSION "1.0")
project(transcoder VERSION ${PROJECT_VERSION})

include_directories(ffmpeg)
include_directories(ffmpeg/include)
include_directories(ffmpeg/include/libavcodec)
include_directories(ffmpeg/include/libavdevice)
include_directories(ffmpeg/include/libavfilter)
include_directories(ffmpeg/include/libavformat)
include_directories(ffmpeg/include/libavutil)
include_directories(ffmpeg/include/libpostproc)
include_directories(ffmpeg/include/libswresample)
include_directories(ffmpeg/include/libswscale)
include_directories(include)

add_library(transcoder SHARED
        src/helper.c
        src/subtitles.c)
set_property(TARGET transcoder PROPERTY C_STANDARD 11)

link_directories(${CMAKE_SOURCE_DIR}/dll)


target_link_libraries(transcoder PRIVATE
        ${CMAKE_SOURCE_DIR}/dll/avcodec.dll
        ${CMAKE_SOURCE_DIR}/dll/avformat.dll
        ${CMAKE_SOURCE_DIR}/dll/avutil.dll
        ${CMAKE_SOURCE_DIR}/dll/avdevice.dll)