//
// Created by anonymus-raccoon on 12/29/19.
//

#pragma once

char *path_getfolder(const char *path);

char *path_getfilename(const char *path);

char *get_extension_from_codec(char *codec);

int path_mkdir(const char *path, int mode);

int path_mkdir_p(const char *path, int mode);