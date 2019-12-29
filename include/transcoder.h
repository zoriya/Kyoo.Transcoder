#pragma once
#include "export.h"
#include "stream.h"

//API int transmux(const char *path, const char *out_path, float *playable_duration);
//
//API int transcode(const char *path, const char *out_path, float *playable_duration);
//
//API stream *get_track_info(const char *path, int *stream_count, int *track_count);

//Take the path of the file and the path of the output directory. It will return the list of subtitle streams in the streams variable. The int returned is the number of subtitles extracted.
API stream *extract_subtitles(char *path, const char *out_path, unsigned *stream_count, unsigned *subtitle_count);

void destroy_stream(stream *s);

API void free_streams(stream *streamsPtr, int count);