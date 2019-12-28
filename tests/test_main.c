//
// Created by anonymus-raccoon on 12/28/19.
//

#include <string.h>
#include "transcoder.h"


int main(int argc, char **argv)
{
    unsigned stream_count = 0;
    unsigned subtitle_count = 0;

    if (argc == 3 && !strcmp(argv[1], "es"))
        extract_subtitles(argv[2], "./", &stream_count, &subtitle_count);
    return (0);
}