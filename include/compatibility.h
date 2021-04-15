//
// Created by Anonymus Raccoon on 16/12/2019.
//

#pragma once

#define _GNU_SOURCE // For asprintf
#include <stdio.h>

#if defined(_WIN32) || defined(WIN32)
	#include <io.h>
	#include <stddef.h>
	#include <stdarg.h>

	char *strndup(const char *str, size_t count);
	int asprintf(char **buffer, const char *fmt, ...);
	int vasprintf(char **buffer, const char *fmt, va_list args);

	#define kmkdir(dir, mode) mkdir(dir)
	#define S_ISDIR(x) (x & S_IFDIR)
#else
	#include <unistd.h>

	#define kmkdir(dir, mode) mkdir(dir, mode)
#endif

#ifdef __MINGW32__
	#define asprintf __mingw_asprintf
#endif