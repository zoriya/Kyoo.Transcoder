cmake_minimum_required(VERSION 3.12)
set(CMAKE_C_STANDARD 11)

set(PROJECT_VERSION "1.0")
project(transcoder VERSION ${PROJECT_VERSION} LANGUAGES C)

include_directories(include)
add_library(transcoder SHARED 
	src/helper.c
	src/info.c
	src/extractor.c
	src/transmuxer.c
	src/path_helper.c
	src/destroyer.c
	src/compatibility.c
)
set_property(TARGET transcoder PROPERTY C_STANDARD 11)
set_property(TARGET transcoder PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(transcoder avformat avcodec avutil)
target_link_libraries(transcoder m pthread)

if(WIN32)
	target_link_libraries(transcoder wsock32 ws2_32)
	target_link_libraries(transcoder Secur32 Secur32)
	target_link_libraries(transcoder Bcrypt Bcrypt)
endif()
