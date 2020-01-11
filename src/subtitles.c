//
// Created by Anonymus Raccoon on 16/12/2019.
//

#include "stream.h"
#include "helper.h"
#include "compatibility.h"
#include "path_helper.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <transcoder.h>

int get_subtitle_data(stream *substream, AVStream *in_stream, const char *file_name, const char *out_path)
{
	AVDictionaryEntry *languageptr = av_dict_get(in_stream->metadata, "language", NULL, 0);
	char *codec = strdup(avcodec_get_name(in_stream->codecpar->codec_id));
	char *extension = get_extension_from_codec(codec);
    char *folder_path;

    if (!extension)
	    return (-1);
	*substream = (stream) {
		NULL,
		languageptr ? strdup(languageptr->value) : NULL,
		codec,
		in_stream->disposition & AV_DISPOSITION_DEFAULT,
		in_stream->disposition & AV_DISPOSITION_FORCED,
		NULL,
		subtitle
	};
	asprintf(&folder_path, "%s/%s", out_path, substream->language);
    if (path_mkdir(folder_path, 0733) < 0) {
        if (!folder_path)
            free(folder_path);
        return (-1);
    }
	asprintf(&substream->path, "%s/%s.%s%s%s%s", folder_path,
			 file_name,
			 substream->language,
			 substream->is_default ? ".default" : "",
			 substream->is_forced ? ".forced" : "",
			 extension);
    free(folder_path);
	if (!substream->path)
		return (-1);
	return (0);
}

int copy_subtitle_stream(AVFormatContext **out_ctx, stream *s, AVFormatContext *int_ctx, AVStream *in_stream)
{
	AVStream *out_stream = NULL;

	if (avformat_alloc_output_context2(out_ctx, NULL, NULL, s->path) < 0) {
		printf("Error: Couldn't create an output file.\n");
		return (-1);
	}
	av_dict_copy(&(*out_ctx)->metadata, int_ctx->metadata, 0);
	out_stream = copy_stream_to_output(*out_ctx, in_stream);
	if (out_stream) {
		av_dump_format(*out_ctx, 0, s->path, true);
		if (open_output_file_for_write(*out_ctx, s->path, NULL) == 0)
			return (0);
	}
	if (*out_ctx && !((*out_ctx)->flags & AVFMT_NOFILE))
		avio_closep(&(*out_ctx)->pb);
	avformat_free_context(*out_ctx);
	fprintf(stderr, "An error occured, cleaning up th output context for the %s stream.\n", s->language);
	return (-1);
}

void write_data(AVFormatContext *int_ctx, AVFormatContext **output_list, unsigned int out_count)
{
	AVPacket pkt;

	while (av_read_frame(int_ctx, &pkt) == 0) {
		AVFormatContext *out_ctx;

		if ((unsigned)pkt.stream_index >= out_count)
			continue;
		out_ctx = output_list[pkt.stream_index];
		if (!out_ctx) {
			av_packet_unref(&pkt);
			continue;
		}
		process_packet(&pkt, int_ctx->streams[pkt.stream_index], out_ctx->streams[0]);
		pkt.stream_index = 0;
		if (av_interleaved_write_frame(out_ctx, &pkt) < 0)
			fprintf(stderr, "Error while writing a packet to the output file.\n");
		av_packet_unref(&pkt);
	}
}

void finish_up(AVFormatContext *int_ctx, AVFormatContext **output_list, unsigned out_count)
{
	avformat_close_input(&int_ctx);

	for (unsigned i = 0; i < out_count; i++) {
		AVFormatContext *out_ctx = output_list[i];

		if (!out_ctx)
			continue;
		av_write_trailer(out_ctx);
		if (!(out_ctx->flags & AVFMT_NOFILE))
			avio_closep(&out_ctx->pb);
		avformat_free_context(out_ctx);
	}
	free(output_list);
}

int split_inputfile(AVFormatContext *int_ctx, AVFormatContext **output_list, stream *streams, char *path, const char *out_path)
{
    int subcount = 0;
    char *file_name = path_getfilename(path);

    if (!file_name)
        return (-1);
    for (unsigned int i = 0; i < int_ctx->nb_streams; i++) {
        AVStream *in_stream = int_ctx->streams[i];

        if (in_stream->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            if (get_subtitle_data(streams + i, in_stream, file_name, out_path) == 0 &&
                copy_subtitle_stream(&output_list[i], streams + i, int_ctx, in_stream) == 0) {
                subcount += 1;
                continue;
            }
            fprintf(stderr,"Couldn't copy the %s subtitle to output\n", streams[i].language);
            destroy_stream(&streams[i]);
        }
        streams[i] = NULLSTREAM;
        output_list[i] = NULL;
    }
    free(file_name);
    return (subcount);
}

stream *extract_subtitles(char *path, const char *out_path, unsigned *stream_count, unsigned *subtitle_count)
{
	AVFormatContext *int_ctx = NULL;
	AVFormatContext **output_list;
    stream *streams;

	if (open_input_context(&int_ctx, path) != 0)
        return (NULL);
	*stream_count = int_ctx->nb_streams;
	streams = calloc(sizeof(stream), *stream_count);
	output_list = malloc(sizeof(AVFormatContext *) * *stream_count);
	if (streams && output_list) {
        *subtitle_count = split_inputfile(int_ctx, output_list, streams, path, out_path);
        if (*subtitle_count >= 0) {
            write_data(int_ctx, output_list, *stream_count);
            finish_up(int_ctx, output_list, *stream_count);
            return (streams);
        }
    }
	*subtitle_count = 0;
    if (streams)
        free_streams(streams, (int)*stream_count);
    if (output_list)
        free(output_list);
    return (NULL);
}
