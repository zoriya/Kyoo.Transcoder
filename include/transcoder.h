#pragma once
#include "export.h"
#include "stream.h"

API int transmux(const char *path, const char *out_path, float *playable_duration);

//API int transcode(const char *path, const char *out_path, float *playable_duration);

API stream *extract_info(const char *path, const char *out_path, unsigned *stream_count, unsigned *track_count);

void destroy_stream(stream *s);

API void free_streams(stream *streamsPtr, int count);