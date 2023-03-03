#include "file_system.h"

int file_info_compare(const void *fir, const void *sec) {
    return strcmp(((struct file_info *) fir)->name, ((struct file_info *) sec)->name);
}

int copy_file(char *clip_board_path, char *clip_board_name) {
    char buf[256];
    snprintf(buf, 256, "cp %s %s", clip_board_path, clip_board_name);
    system(buf);

    return 0;
}

int get_files_from_directory(struct vector *vec, const char *dir_name, int hidden_files) {
    DIR *directory = opendir(dir_name);
    struct dirent *ent;

    while ((ent = readdir(directory))) {
        if (strcmp(ent->d_name, ".") == 0 ||
            (!hidden_files && ent->d_name[0] == '.' && strcmp(ent->d_name, "..") != 0)) {
            continue;
        }
        struct stat dir_info;
        lstat(ent->d_name, &dir_info);
        struct file_info info;

        strcpy(info.name, ent->d_name);
        info.fstat = dir_info;
        vector_push_back(vec, &info);
    }

    closedir(directory);
    qsort(vec->buf, vec->size, vec->elem_sizeof, file_info_compare);

    return 0;
}

int exist(struct vector *vec, const char *name) {
    for (int i = 0; i < vec->size; ++i) {
        struct file_info *info;
        vector_get(vec, i, (void **) &info);
        if (strcmp(info->name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

void clear_screen() {
    system("clear");
}

int display_file_system(struct vector *vec, int cursor_pos, int *display_start) {
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    int window_width = window.ws_col;
    int window_height = window.ws_row;

    if (cursor_pos >= *display_start + window_height - 2) {
        *display_start = cursor_pos - window_height + 3;
    }

    if (cursor_pos < *display_start) {
        *display_start = cursor_pos;
    }

    printf("\033[30m\033[107m%*s%*s\033[0m\n", -(window_width - 20), "   Name", 20, "Modified ");
    printf("\033[97m\033[40m");

    struct tm current_time_buf;
    time_t current_time = time(NULL);
    localtime_r(&current_time, &current_time_buf);

    int stop = *display_start + window_height - 2;
    if (vec->size < stop) {
        stop = vec->size;
    }

    for (int i = *display_start; i < stop; ++i) {
        struct file_info *info;
        vector_get(vec, i, (void **) &info);

        struct tm time_buf;
        char formatted_time_buf[30];
        localtime_r(&info->fstat.st_mtime, &time_buf);
        if (time_buf.tm_year == current_time_buf.tm_year) {
            strftime(formatted_time_buf, sizeof(formatted_time_buf), "%b%3e %H:%M", &time_buf);
        } else {
            strftime(formatted_time_buf, sizeof(formatted_time_buf), "%b%3e  %Y", &time_buf);
        }

        char *pref = "   ";

        if (i == cursor_pos) {
            pref = "-> ";
        }
        printf("%s", pref);
        if (S_ISDIR(info->fstat.st_mode)) {
            printf("\033[34m");
        } else if (S_ISLNK(info->fstat.st_mode)) {
            printf("\033[96m");
        } else if (S_ISFIFO(info->fstat.st_mode)) {
            printf("\033[33m");
        }
        printf("%s", info->name);
        printf("\033[97m");
        printf("%*s\n", (int) (window_width - strlen(info->name) - 3), formatted_time_buf);
    }
    printf("\033[0m");

    return vec->size;
}

int update_screen(struct vector *vec, int cursor_pos, int *display_start, int hidden_files) {
    vector_clear(vec);
    get_files_from_directory(vec, ".", hidden_files);
    clear_screen();
    return display_file_system(vec, cursor_pos, display_start);
}
