//
// Created by Anonymus Raccoon on 20/12/2019.
//

#include <stdio.h>
#include "stream.h"
#include "helper.h"

int init()
{
	puts("Kyoo's transcoder initiated.");
	return sizeof(stream);
}

stream *get_track_info(const char *path, unsigned *stream_count, unsigned *track_count)
{
	AVFormatContext *ctx = NULL;
	stream *streams;

	if (open_input_context(&ctx, path) != 0)
		return NULL;
	*stream_count = ctx->nb_streams;
	*track_count = 0;
	streams = malloc(sizeof(stream) * *stream_count);
	for (unsigned i = 0; i < *stream_count; i++) {
		AVStream *stream = ctx->streams[i];
		const AVCodecParameters *codecpar = stream->codecpar;

		if (type_fromffmpeg(codecpar->codec_type) != none) {
			AVDictionaryEntry *languageptr = av_dict_get(stream->metadata, "language", NULL, 0);

			*track_count += 1;
			streams[i] = (struct stream){
				NULL,
				languageptr ? strdup(languageptr->value) : NULL,
				strdup(avcodec_get_name(codecpar->codec_id)),
				stream->disposition & AV_DISPOSITION_DEFAULT,
				stream->disposition & AV_DISPOSITION_FORCED,
				strdup(path),
				type_fromffmpeg(codecpar->codec_type)
			};
		}
		else
			streams[i] = NULLSTREAM;
	}
	avformat_close_input(&ctx);
	return streams;
}