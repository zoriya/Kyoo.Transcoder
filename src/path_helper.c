//
// Created by anonymus-raccoon on 12/29/19.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *strrnchr(const char *str, int c, int occ_to_skip)
{
    const char *ptr = str + strlen(str);

    while (ptr != str) {
        if (*str == c) {
            occ_to_skip--;
            if (occ_to_skip == 0)
                return ((char *)str);
        }
        str--;
    }
    return (NULL);
}

char *path_getfolder(const char *path)
{
    char *start;
    char *end;
    char *folder;

    start = strrnchr(path, '/', 1);
    end = strrchr(path, '/');
    if (!end)
        return (NULL);
    folder = strndup(start, end - start);
    return (folder);
}

char *get_extension_from_codec(char *codec)
{
    if (!codec)
        return (NULL);
    if (!strcmp(codec, "subrip"))
        return(".srt");
    else if (!strcmp(codec, "ass"))
        return(".ass");
    else {
        printf("Unsupported subtitle codec: %s.\n", codec);
        free(codec);
        return (NULL);
    }
}