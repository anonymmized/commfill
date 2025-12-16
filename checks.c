#include <stdio.h>
#include <string.h>
#include "checks.h"

void get_slash(char *filename_sl, char *filename) {
    char *last_slash = strrchr(filename, '/');
    if (last_slash) {
        strcpy(filename_sl, last_slash + 1);
    } else {
        strcpy(filename_sl, filename);
    }
}

int get_filename(char *line, int lim) {
    printf("Enter the filename (with extension): ");
    if (fgets(line, lim, stdin) == NULL) {
        line[0] = '\0';
        return 0;
    }
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    return (int)len;
}

int check_filename(char *filename) {
    return (strchr(filename, '.') == NULL);
}