//
// Created by Anonymus Raccoon on 16/12/2019.
//

#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <libavformat/avformat.h>

typedef enum
{
	none = 0,
	video = 1,
	audio = 2,
	subtitle = 3,
	font = 4
} type;

typedef struct stream
{
	char *title;
	char *language;
	char *codec;
	bool is_default;
	bool is_forced;
	char *path;
	type type;
} stream;

void extract_track(stream *track,
                   const char *out_path,
                   AVStream *stream,
                   AVFormatContext *in_ctx,
                   AVFormatContext **out_ctx);

void extract_font(stream *font, const char *out_path, AVStream *stream);