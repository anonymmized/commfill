#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_func.h"

int main(int argc, char *argv[]) {
    char filename[MAXLINE];
    int remove_old = 0;
    int file_input = 0;
    char *fixed = NULL;
    int get_name = 0;
    int aligned = 0;
    char *file_input_s = NULL;
    char **arg = argv + 1;
    int remaining = argc - 1;
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
                    arg += 1;
                    remaining -= 2;
                    continue;
                }
            } else if (strcmp(current, "-a") == 0) {
                aligned = 1;
                arg++;
                remaining--;
                continue;
            } else {
                printf("Unknown option: %s\n", current);
            }
        } else {
            break;
        }
    }
    int fixed_i = 0;
    if (fixed) {
        fixed_i = atoi(fixed);
    }
    char filename_sl[MAXLINE];
    if (file_input == 1) {
        size_t len = strlen(file_input_s);
        if (len >= MAXLINE) {
            printf("Error: filename too long\n");
            return 1;
        }
        strcpy(filename, file_input_s);
        get_name = 1;
    }
    if (get_name == 0) {
        int len = get_filename(filename, MAXLINE);
        if (len == 0) {
            printf("No filename entered.\n");
            return 1;
        }
    } else if (get_name == 1) {
        int len = strlen(filename);
        if (len == 0) {
            printf("No filename entered.\n");
            return 1;
        }
    }
    
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