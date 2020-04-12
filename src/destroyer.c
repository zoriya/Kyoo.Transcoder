//
// Created by anonymus-raccoon on 12/29/19.
//

#include "stream.h"
#include <stdlib.h>

void destroy_stream(stream *s)
{
	if (s->title)
		free(s->title);
	if (s->language)
		free(s->language);
	if (s->codec)
		free(s->codec);
	if (s->path)
		free(s->path);
}

void free_streams(stream *s, int count)
{
	for (unsigned i = 0; i < count; i++)
		destroy_stream(s + i);
	free(s);
}