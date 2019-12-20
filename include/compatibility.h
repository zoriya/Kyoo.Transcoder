//
// Created by Anonymus Raccoon on 16/12/2019.
//

#pragma once

#include <stdio.h>

#ifdef __WIN32__
	#define kmkdir(dir, mode) mkdir(dir)
#else
	#define kmkdir(dir, mode) mkdir(dir, mode)
#endif

#ifdef __MINGW32__
	#define asprintf __mingw_asprintf
#endif