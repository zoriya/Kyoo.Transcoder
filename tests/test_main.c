//
// Created by anonymus-raccoon on 12/28/19.
//

#include <string.h>
#include <stdio.h>
#include "transcoder.h"
#include "stream.h"


int main(int argc, char **argv)
{
    unsigned stream_count = 0;
    unsigned subtitle_count = 0;
    float playable_duration;
    stream *streams;

    if (argc == 3 && !strcmp(argv[1], "info")) {
        streams = get_track_info(argv[2],  &stream_count, &subtitle_count);
        free_streams(streams, stream_count);
    }
    else if (argc == 3 && !strcmp(argv[1], "subextr")) {
        streams = extract_subtitles(argv[2], ".", &stream_count, &subtitle_count);
        free_streams(streams, stream_count);
    }
    else if (argc == 4 && !strcmp(argv[1], "transmux"))
        return (transmux(argv[2], argv[3], &playable_duration));
    else
        printf("\nUsage:\n\n\
    %s INFO video_path - Test info prober\n\
    %s subextr video_path - Test subtitle extractions\n\
    %s transmux video_path m3u8_output_file - Test transmuxing\n", argv[0], argv[0], argv[0]);
    return (0);
}