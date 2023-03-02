#include <dirent.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include "file_open.h"

void handlers_init(struct vector *ext_list) {
    DIR *directory = opendir("../extensions");
    struct dirent *ent;

    while ((ent = readdir(directory))) {
        char buf[512];
        snprintf(buf, sizeof(buf), "../%s", ent->d_name);
        void *handle = dlopen(buf, RTLD_LAZY);
        if (handle == NULL) {
            continue;
        }
        const char *(*get_extension)() = dlsym(handle, "get_extension");
        if (get_extension == NULL) {
            continue;
        }

        const char *ext = get_extension();
        struct file_handler handler;
        strcpy(handler.extension, ext);
        handler.function_pointer = dlsym(handle, "open_file");

        vector_push_back(ext_list, &handler);
    }
}

int compare_extensions(const char *file_name, const char *extension) {
    size_t file_name_size = strlen(file_name);
    size_t extension_size = strlen(extension);

    if (file_name_size < extension_size) {
        return 0;
    }

    if (strcmp(file_name + (file_name_size - extension_size), extension) == 0) {
        return 1;
    }
    return 0;
}

void *get_handler_for_opening(struct vector *ext_list, const char *name) {
    for (int i = 0; i < ext_list->size; ++i) {
        struct file_handler *handler;
        vector_get(ext_list, i, (void **) &handler);
        if (compare_extensions(name, handler->extension)) {
            return handler->function_pointer;
        }
    }
    return NULL;
}
