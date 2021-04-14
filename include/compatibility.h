//
// Created by Anonymus Raccoon on 16/12/2019.
//

#pragma once

#define _GNU_SOURCE // For asprintf
#include <stdio.h>

#if defined(_WIN32) || defined(WIN32)
	#define kmkdir(dir, mode) mkdir(dir)
	#include <io.h>
	#include <stddef.h>
	char *strndup(const char *str, size_t count);
#else
	#define kmkdir(dir, mode) mkdir(dir, mode)
	#include <unistd.h>
#endif

#ifdef __MINGW32__
	#define asprintf __mingw_asprintf
#endif