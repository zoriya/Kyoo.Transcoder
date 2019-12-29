#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef enum
{
    none = 0,
    video = 1,
    audio = 2,
    subtitle = 3
} type;

typedef struct stream
{
	char *title;
	char *language;
	char *codec;
	bool is_default;
	bool is_forced;
	char *path;
	type type;
} stream;

#define NULLSTREAM (struct stream) { \
	NULL, \
	NULL, \
	NULL, \
	false, \
	false, \
	NULL, \
	none \
}
