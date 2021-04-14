//
// Created by Zoe Roux on 2021-04-15.
//

#include "compatibility.h"
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(WIN32)
char *strndup(const char *str, size_t count)
{
	size_t len = strnlen(str, count);
	char *ret = malloc(sizeof(char) * (len + 1));

	if (!ret)
		return NULL;
	ret[len] = '\0';
	memcpy(ret, str, len);
	return ret;
}
#endif