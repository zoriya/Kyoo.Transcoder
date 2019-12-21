//
// Created by Anonymus Raccoon on 20/12/2019.
//

#include "transcoder.h"
#include "helper.h"
#include "compatibility.h"


int transmux(const char *path, const char *out_path, float *playable_duration)
{
	AVFormatContext *in_ctx = NULL;
	AVFormatContext *out_ctx = NULL;
	AVPacket pkt;
	AVDictionary *options = NULL;
	int *stream_map;
	int stream_count;
	int ret = 0;
	char *seg_path;

	*playable_duration = 0;
	if (open_input_context(&in_ctx, path) != 0)
		return (-1);
	if (avformat_alloc_output_context2(&out_ctx, NULL, NULL, out_path) < 0) {
		fprintf(stderr, "Error: Couldn't create an output file.\n");
		return (-1);
	}
	stream_map = malloc(sizeof(int) * in_ctx->nb_streams);
	if (!strema_map)
		return (-1);
	stream_count = 0;
	for (unsigned i = 0; i < in_ctx->nb_streams; i++) {
		AVStream *stream = in_ctx->streams[i];

		if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO || stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			stream_map[i] = stream_count;
			stream_count++;
			if (copy_stream_to_output(out_ctx, stream) == NULL)
				return 1;
		} else
			stream_map[i] = -1;
	}

	av_dump_format(out_ctx, 0, out_path, true);
	asprintf(&segpath, "%s/segments/", strchr(out_path, '/'));
	if (!seg_path || kmkdir(seg_path, 0733) < 0)
		return (-1);
	std::filesystem::create_directory(seg_path);
	av_dict_set(&options, "hls_segment_filename", seg_path.append("%v-%03d.ts").c_str(), 0);
	av_dict_set(&options, "hls_base_url", "segment/", 0);
	av_dict_set(&options, "hls_list_size", "0", 0);
	av_dict_set(&options, "streaming", "1", 0);

	if (open_output_file_for_write(out_ctx, out_path, &options) != 0)
		return 1;

	while (av_read_frame(in_ctx, &pkt) == 0)
	{
		if ((unsigned int)pkt.stream_index >= in_ctx->nb_streams || stream_map[pkt.stream_index] < 0)
		{
			av_packet_unref(&pkt);
			continue;
		}

		stream = in_ctx->streams[pkt.stream_index];
		pkt.stream_index = stream_map[pkt.stream_index];
		process_packet(pkt, stream, out_ctx->streams[pkt.stream_index]);
		if (pkt.stream_index == 0)
			*playable_duration += pkt.duration * (float)out_ctx->streams[pkt.stream_index]->time_base.num / out_ctx->streams[pkt.stream_index]->time_base.den;

		if (av_interleaved_write_frame(out_ctx, &pkt) < 0)
			std::cout << "Error while writing a packet to the output file." << std::endl;

		av_packet_unref(&pkt);
	}

	av_dict_free(&options);
	av_write_trailer(out_ctx);
	avformat_close_input(&in_ctx);

	if (out_ctx && !(out_ctx->oformat->flags & AVFMT_NOFILE))
		avio_close(out_ctx->pb);
	avformat_free_context(out_ctx);
	delete[] stream_map;

	if (ret < 0 && ret != AVERROR_EOF)
		return 1;

	return 0;
}