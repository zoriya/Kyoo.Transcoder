//
// Created by anonymus-raccoon on 10/27/20.
//

#include "compatibility.h"
#include "path_helper.h"
#include "stream.h"
#include "helper.h"
#include <stdlib.h>
#include <libavformat/avformat.h>
#include <fcntl.h>
#include <sys/stat.h>

// @return -1 on error, 1 if track has already been extracted, 0 if the track does not exist.
int create_out_path(stream *track, const char *out_path, int track_id)
{
	char *folder_path;

	asprintf(&folder_path, "%s/Subtitles/%s", out_path, track->language ? track->language : "und");
	if (path_mkdir_p(folder_path, 0775))
		return -2;
	char *extension = get_extension_from_codec(track->codec);
	char *file_name = path_getfilename(track->path);

	if (!extension || !file_name) {
		free(folder_path);
		free(file_name);
		return -2;
	}
	free(track->path);

	char identifier[8];
	if (!track->language)
		snprintf(identifier, 8, "%d", track_id);
	asprintf(&track->path, "%s/%s.%s%s%s%s",
	         folder_path,
	         file_name,
	         track->language ? track->language : identifier,
	         track->is_default ? ".default" : "",
	         track->is_forced ? ".forced" : "",
	         extension);
	free(folder_path);
	free(file_name);
	if (!track->path)
		return -2;
	struct stat s;
	return stat(track->path, &s) == 0 && s.st_size > 0;
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
	fprintf(stderr, "An error occurred, cleaning up the output context for the %s stream.\n", s->language);
	*out_ctx = NULL;
	return -1;
}

void extract_track(stream *track,
                   const char *out_path,
                   AVStream *stream,
                   AVFormatContext *in_ctx,
                   AVFormatContext **out_ctx,
                   bool reextract)
{
	int ret = create_out_path(track, out_path, stream->id);
	if (ret == 0 || (reextract && ret == 1))
		extract_stream(out_ctx, track, in_ctx, stream);
}

void extract_attachment(stream *font, const char *out_path, AVStream *stream)
{
	AVDictionaryEntry *filename = av_dict_get(stream->metadata, "filename", NULL, 0);

	if (!filename)
		return;
	free(font->path);
	font->path = malloc((strlen(out_path) + 18 + strlen(filename->value)) * sizeof(char));
	if (!font->path)
		return;
	strcpy(font->path, out_path);
	strcat(font->path, "/Attachments/");
	if (path_mkdir(font->path, 0755) < 0) {
		free(font->path);
		return;
	}
	strcat(font->path, filename->value);
	size_t count = strchr(filename->value, '.') - filename->value;
	if (count > 0)
		font->title = strndup(filename->value, count);

	int fd = open(font->path, O_WRONLY | O_CREAT, 0644);
	if (fd == -1) {
		perror("Kyoo couldn't extract an attachment.");
		return;
	}
	write(fd, stream->codecpar->extradata, stream->codecpar->extradata_size);
	close(fd);
}

void extract_chapters(AVFormatContext *ctx, const char *out_path)
{
	if (ctx->nb_chapters == 0)
		return;

	const char *filename = strrchr(ctx->url, '/');
	char *path = malloc((strlen(filename) + strlen(out_path) + 15) * sizeof(char));
	char *tmp;

	if (!path)
		return;
	strcpy(path, out_path);
	strcat(path, "/Chapters/");
	if (path_mkdir_p(path, 0755) < 0)
		return;
	strcat(path, filename);
	tmp = strrchr(path, '.');
	if (tmp)
		*tmp = '\0';
	strcat(path, ".txt");

	FILE *file = fopen(path, "w");

	for (unsigned i = 0; i < ctx->nb_chapters; i++) {
		AVDictionaryEntry *name = av_dict_get(ctx->chapters[i]->metadata, "title", NULL, 0);
		if (!name)
			continue;
		const AVChapter *chapter = ctx->chapters[i];
		if (chapter->start == AV_NOPTS_VALUE || chapter->end == AV_NOPTS_VALUE)
			continue;
		double start = chapter->start * av_q2d(chapter->time_base);
		double end = chapter->end * av_q2d(chapter->time_base);
		fprintf(file, "%f %f %s\n", start, end, name->value);
	}
	fclose(file);
	free(path);
}