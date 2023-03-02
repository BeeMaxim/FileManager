#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>

#include "file_system.h"
#include "file_open.h"

enum KEY_CODE {
    ENTER = 10,
    DELETE = 100,
    QUIT = 113,
    COPY = 99,
    CUT = 120,
    PASTE = 118,
    HIDE = 104
};

int main() {
    struct vector *vec = vector_init(sizeof(struct file_info));
    struct vector *ext_list = vector_init(sizeof(struct file_handler));

    handlers_init(ext_list);

    int cursor_pos = 0;
    int hidden_files = 0;
    int file_count = 0;
    int cut_file = 0;
    char clip_board_path[256];
    char clip_board_name[256];

    file_count = update_screen(vec, cursor_pos, hidden_files);

    struct termios old_attr, new_attr;
    tcgetattr(STDIN_FILENO, &old_attr);
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);

    while (1) {
        int ch;
        ch = getchar();

        if (ch == 27) {
            ch = getchar();
            if (ch == 91) {
                ch = getchar();
                if (ch == 65) {
                    if (cursor_pos > 0) {
                        --cursor_pos;
                    }
                    clear_screen();
                    file_count = display_file_system(vec, cursor_pos);
                } else if (ch == 66) {
                    if (cursor_pos < file_count - 1) {
                        ++cursor_pos;
                    }
                    clear_screen();
                    file_count = display_file_system(vec, cursor_pos);
                }
            }
        } else if (ch == ENTER) {
            struct file_info *info;
            vector_get(vec, cursor_pos, (void **) &info);

            if (S_ISDIR(info->fstat.st_mode)) {
                chdir(info->name);
                cursor_pos = 0;
                file_count = update_screen(vec, cursor_pos, hidden_files);
                continue;
            }

            int (*open_file)(const char *) = get_handler_for_opening(ext_list, info->name);

            if (open_file == NULL) {
                continue;
            }
            open_file(info->name);
        } else if (ch == COPY || ch == CUT) {
            struct file_info *info;
            vector_get(vec, cursor_pos, (void **) &info);

            getcwd(clip_board_path, sizeof(clip_board_path));
            snprintf(clip_board_path + strlen(clip_board_path), sizeof(clip_board_path) - strlen(clip_board_path),
                     "/%s", info->name);
            snprintf(clip_board_name, sizeof(clip_board_name), "%s", info->name);
            if (ch == CUT) {
                cut_file = 1;
            }
        } else if (ch == PASTE) {
            if (exist(vec, clip_board_name)) {
                continue;
            }
            copy_file(clip_board_path, clip_board_name);
            file_count = update_screen(vec, cursor_pos, hidden_files);
            if (cut_file) {
                unlink(clip_board_path);
            }
            cut_file = 0;
        } else if (ch == HIDE) {
            hidden_files ^= 1;
            file_count = update_screen(vec, cursor_pos, hidden_files);
        } else if (ch == DELETE) {
            struct file_info *info;
            vector_get(vec, cursor_pos, (void **) &info);

            if (strcmp(info->name, "..") == 0) {
                continue;
            }
            unlink(info->name);

            vector_clear(vec);
            get_files_from_directory(vec, ".", hidden_files);
            if (cursor_pos + 1 == file_count) {
                --cursor_pos;
            }
            clear_screen();
            file_count = display_file_system(vec, cursor_pos);
        } else if (ch == QUIT) {
            break;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
    vector_free(vec);
    vector_free(ext_list);

    return 0;
}
