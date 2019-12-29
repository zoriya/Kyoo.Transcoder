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
    stream *streams;

    if (argc == 3 && !strcmp(argv[1], "subextract")) {
        streams = extract_subtitles(argv[2], ".", &stream_count, &subtitle_count);
        free_streams(streams, stream_count);
    } else
        printf("\nUsage:\n\n\
    %s subextr video_path - Test subtitle extractions\n", argv[0]);
    return (0);
}