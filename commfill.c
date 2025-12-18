#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_func.h"

#define MAXLINE 1000
#define MAXFILES 500

int parse_single(char *fixed, int file_input, int aligned, int remove_old, char *filename, char *file_input_s) {
    int fixed_i = 0;
    char filename_sl[MAXLINE];
    
    if (fixed) {
        fixed_i = atoi(fixed);
    }
    
    size_t len = strlen(file_input_s);
    if (len >= MAXLINE) {
        printf("Error: filename too long\n");
        return 1;
    }
    strcpy(filename, file_input_s);
    
    get_slash(filename_sl, filename);
    printf("Filename: %s\n", filename_sl);

    if (check_filename(filename_sl)) {
        printf("Warning: filename has no extension.\n");
    }

    if (remove_old == 1) {
        remove_old_comms(filename);
        printf("Old comments were deleted\n");
        return 0;
    }
    size_t max_len = file_parse(filename);
    printf("Max line length: %zu\n", max_len);

    if (max_len > 0) {
        if (aligned == 1) {
            create_edited_file(max_len, filename, fixed_i);
            printf("File updated and created successfully.\n");
        } else {
            edit_file(max_len, filename, fixed_i);
            printf("File updated successfully.\n");
        }
    } else {
        printf("File is empty or could not be processed.\n");
    }
    return 0;
}

int parse_dir(char filenames[][MAXLINE], char *dir_input_s, char *dirname, int remove_old, char *fixed, int aligned) {
    int fixed_i = 0;
    if (fixed) {
        fixed_i = atoi(fixed);
    }
    size_t len = strlen(dir_input_s);
    if (len >= MAXLINE) {
        printf("Error: dir name is too long\n");
        return 1;
    }
    strcpy(dirname, dir_input_s);
    size_t filenames_len = filenames_init(filenames, dir_input_s);
    char full_path[MAXLINE];
    if (remove_old == 1) {
        for (int i = 0; i < filenames_len; i++) {
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_input_s, filenames[i]);
            remove_old_comms(full_path);
            printf("Old comments were deleted in %s\n", full_path);
        }
        return 0;
    }
    for (int i = 0; i < filenames_len; i++) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_input_s, filenames[i]);
        size_t max_len = file_parse(full_path);
        printf("Max line length in %s: %zu\n", full_path, max_len);
        if (max_len > 0) {
            if (aligned == 1) {
                create_edited_file(max_len, full_path, fixed_i);
                printf("File %s updated and created successfully.\n", full_path);
            } else {
                edit_file(max_len, full_path, fixed_i);
                printf("File %s updated successfully.\n", full_path);
            }
        } else {
            printf("File %s is empty or could not be processed.\n", filenames[i]);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    char filename[MAXLINE];
    int remove_old = 0;
    int file_input = 0;
    char *fixed = NULL;
    int aligned = 0;
    int get_name = 0;
    char *file_input_s = NULL;
    char **arg = argv + 1;
    int remaining = argc - 1;
    char filenames[MAXFILES][MAXLINE];
    char dirname[MAXLINE];
    char *dir_input_s = NULL;
    int dir_input = 0;
    while (remaining > 0) {
        char *current = *arg;
        if (current[0] == '-' && current[1] != '\0') {
            if (strcmp(current, "-i") == 0) {
                if (remaining > 1) {
                    file_input = 1;
                    file_input_s = *(arg + 1);
                    arg += 2;
                    remaining -= 2;
                    continue;
                } else {
                    printf("Error: option -i requires a filename\n");
                    return 1;
                }
            } else if (strcmp(current, "-r") == 0) {
                remove_old = 1;
                arg++;
                remaining--;
                continue;
            } else if (strcmp(current, "-w") == 0) {
                if (remaining > 1) {
                    fixed = *(arg + 1);
                    arg += 2;
                    remaining -= 2;
                    continue;
                }
            } else if (strcmp(current, "-a") == 0) {
                aligned = 1;
                arg++;
                remaining--;
                continue;
            } else if (strcmp(current, "-d") == 0) {
                if (remaining > 1) {
                    dir_input = 1;
                    dir_input_s = *(arg + 1);
                    arg += 2;
                    remaining -= 2;
                    continue;
                } else {
                    printf("Error: option -d requires a dir name\n");
                    return 1;
                }
            }
            else {
                printf("Unknown option: %s\n", current);
            }
        } else {
            break;
        }
    } 
    if (dir_input == 1) {
        parse_dir(filenames, dir_input_s, dirname, remove_old, fixed, aligned);
    } else {
        parse_single(fixed, file_input, aligned, remove_old, filename, file_input_s);
    }
    
    return 0;
}