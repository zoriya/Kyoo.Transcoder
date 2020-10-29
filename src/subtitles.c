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
#include <fcntl.h>

// @return -2 on error, -1 if track has alreaady been extracted, 0 on success.
int create_out_path(stream *track, const char *out_path)
{
	char *extension = get_extension_from_codec(track->codec);
	char *folder_path;
	char *file_name;

	if (!extension)
		return -2;
	file_name = path_getfilename(track->path);
	asprintf(&folder_path, "%s/%s", out_path, track->language);
	if (path_mkdir(folder_path, 0733) < 0) {
		free(folder_path);
		free(file_name);
		return -2;
	}
	free(track->path);
	asprintf(&track->path, "%s/%s.%s%s%s%s", folder_path,
	         file_name,
	         track->language,
	         track->is_default ? ".default" : "",
	         track->is_forced ? ".forced" : "",
	         extension);
	free(folder_path);
	free(file_name);
	if (!track->path)
		return -2;
	return access(track->path, F_OK) == 0 ? -1 : 0;
}

int extract_stream(AVFormatContext **out_ctx, stream *s, AVFormatContext *int_ctx, AVStream *in_stream)
{
	AVStream *out_stream = NULL;

	if (avformat_alloc_output_context2(out_ctx, NULL, NULL, s->path) < 0) {
		fprintf(stderr, "Error: Couldn't create an output file.\n");
		return -1;
	}

	av_dict_copy(&(*out_ctx)->metadata, int_ctx->metadata, 0);
	out_stream = copy_stream_to_output(*out_ctx, in_stream);
	if (out_stream && open_output_file_for_write(*out_ctx, s->path, NULL) == 0)
		return 0;

	if (*out_ctx && !((*out_ctx)->flags & AVFMT_NOFILE))
		avio_closep(&(*out_ctx)->pb);
	avformat_free_context(*out_ctx);
	fprintf(stderr, "An error occured, cleaning up th output context for the %s stream.\n", s->language);
	return -1;
}

void extract_track(stream *track,
                   const char *out_path,
                   AVStream *stream,
                   AVFormatContext *in_ctx,
                   AVFormatContext **out_ctx)
{
	if (create_out_path(track, out_path) != 0)
		return;
	extract_stream(out_ctx, track, in_ctx, stream);
}

void extract_font(stream *font, const char *out_path, AVStream *stream)
{
	AVDictionaryEntry *filename = av_dict_get(stream->metadata, "filename", NULL, 0);

	if (!filename)
		return;
	free(font->path);
	font->path = malloc((strlen(out_path) + 7 + strlen(filename->value)) * sizeof(char));
	if (!font->path)
		return;
	strcpy(font->path, out_path);
	strcat(font->path, "/fonts/");
	if (path_mkdir(font->path, 0733) < 0)
		return free(font->path);
	strcat(font->path, filename->value);
	int count = strchr(filename->value, '.') - filename->value;
	if (count > 0)
		font->title = strndup(filename->value, count);

	int fd = open(font->path, O_WRONLY | O_CREAT);
	if (fd == -1)
		return perror("Kyoo couldn't extract a subtitle's font");
	write(fd, stream->codecpar->extradata, stream->codecpar->extradata_size);
	close(fd);
}