//
// Created by Anonymus Raccoon on 16/12/2019.
//

#pragma once

#define _GNU_SOURCE // For asprintf
#include <stdio.h>

#ifdef __WIN32__
	#define kmkdir(dir, mode) mkdir(dir)
	#include <io.h>
#else
	#define kmkdir(dir, mode) mkdir(dir, mode)
	#include <unistd.h>
#endif

#ifdef __MINGW32__
	#define asprintf __mingw_asprintf
#endif