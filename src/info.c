//
// Created by Anonymus Raccoon on 20/12/2019.
//

#include <stdio.h>
#include "stream.h"
#include "helper.h"

int init()
{
    printf("Kyoo Transcoder INITIALIZED.\n");
    return (42);
}

stream *get_track_info(const char *path, int *stream_count, int *track_count)
{
	AVFormatContext *ctx = NULL;
	stream *streams;

	if (open_input_context(&ctx, path) != 0)
		return (NULL);
	*stream_count = ctx->nb_streams;
	*track_count = 0;
	streams = malloc(sizeof(stream) * *stream_count);

	for (int i = 0; i < *stream_count; i++) {
		AVStream *stream = ctx->streams[i];
		const AVCodecParameters *codecpar = stream->codecpar;

		if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO || codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			AVDictionaryEntry *languageptr = av_dict_get(stream->metadata, "language", NULL, 0);

			*track_count += 1;
			streams[i] = (struct stream){
				codecpar->codec_type == AVMEDIA_TYPE_VIDEO ? "VIDEO" : NULL,
				languageptr ? strdup(languageptr->value) : NULL,
				strdup(avcodec_get_name(codecpar->codec_id)),
				stream->disposition & AV_DISPOSITION_DEFAULT,
				stream->disposition & AV_DISPOSITION_FORCED,
				strdup(path)
			};
		}
		else
			streams[i] = NULLSTREAM;
	}
	avformat_close_input(&ctx);
	return (streams);
}