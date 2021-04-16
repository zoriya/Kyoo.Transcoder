//
// Created by Anonymus Raccoon on 16/12/2019.
//

#pragma once

#define _GNU_SOURCE

#if defined(_WIN32) || defined(WIN32)
	#define _CRT_SECURE_NO_WARNINGS
	#define _CRT_NONSTDC_NO_DEPRECATE


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

#include <stdio.h>