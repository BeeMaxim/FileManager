#pragma once

#include "vector.h"

struct file_handler {
    char extension[256];
    void *function_pointer;
};

void handlers_init(struct vector *ext_list);

int compare_extensions(const char *file_name, const char *extension);

void *get_handler_for_opening(struct vector *ext_list, const char *name);
