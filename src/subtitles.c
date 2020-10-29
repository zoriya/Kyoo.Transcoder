//
// Created by anonymus-raccoon on 10/27/20.
//

#include "compatibility.h"
#include "path_helper.h"
#include "stream.h"
#include "helper.h"
#include <malloc.h>
#include <libavformat/avformat.h>
#include <unistd.h>

// @return -2 on error, -1 if subtitle has alreaady been extracted, 0 on success.
int create_out_path(stream *subtitle, const char *out_path)
{
	char *extension = get_extension_from_codec(subtitle->codec);
	char *folder_path;
	char *file_name;

	if (!extension)
		return -2;
	file_name = path_getfilename(subtitle->path);
	asprintf(&folder_path, "%s/%s", out_path, subtitle->language);
	if (path_mkdir(folder_path, 0733) < 0) {
		free(folder_path);
		free(file_name);
		return -2;
	}
	free(subtitle->path);
	asprintf(&subtitle->path, "%s/%s.%s%s%s%s", folder_path,
	         file_name,
	         subtitle->language,
	         subtitle->is_default ? ".default" : "",
	         subtitle->is_forced ? ".forced" : "",
	         extension);
	free(folder_path);
	free(file_name);
	if (!subtitle->path)
		return -2;
	return access(subtitle->path, F_OK) == 0 ? -1 : 0;
}

int copy_subtitle_stream(AVFormatContext *out_ctx, stream *s, AVFormatContext *int_ctx, AVStream *in_stream)
{
	AVStream *out_stream = NULL;

	if (avformat_alloc_output_context2(&out_ctx, NULL, NULL, s->path) < 0) {
		fprintf(stderr, "Error: Couldn't create an output file.\n");
		return -1;
	}

	av_dict_copy(&out_ctx->metadata, int_ctx->metadata, 0);
	out_stream = copy_stream_to_output(out_ctx, in_stream);
	if (out_stream && open_output_file_for_write(out_ctx, s->path, NULL) == 0)
		return 0;

	if (out_ctx && !(out_ctx->flags & AVFMT_NOFILE))
		avio_closep(&out_ctx->pb);
	avformat_free_context(out_ctx);
	fprintf(stderr, "An error occured, cleaning up th output context for the %s stream.\n", s->language);
	return -1;
}

void extract_subtitle(stream *subtitle,
					  const char *out_path,
					  AVStream *stream,
					  AVFormatContext *in_ctx,
					  AVFormatContext *out_ctx)
{
	if (create_out_path(subtitle, out_path) != 0)
		return;
	copy_subtitle_stream(out_ctx, subtitle, in_ctx, stream);
}