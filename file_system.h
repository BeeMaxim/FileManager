#pragma once

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>

#include "vector.h"

struct file_info {
    char name[256];
    struct stat fstat;
};

int file_info_compare(const void *fir, const void *sec);

int copy_file(char *clip_board_path, char *clip_board_name);

int get_files_from_directory(struct vector *vec, const char *dir_name, int hidden_files);

int exist(struct vector *vec, const char *name);

void clear_screen();

int display_file_system(struct vector *vec, int cursor_pos);

int update_screen(struct vector *vec, int cursor_pos, int hidden_files);
