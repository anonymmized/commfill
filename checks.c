#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "checks.h"
#define MAXFILES 500
#define MAXLINE 1000

int filenames_init(char files_mas[][MAXLINE], char *dir_path) {
    int i = 0;
    DIR *dir;
    struct dirent *entry;
    dir = opendir(dir_path);
    if (!dir) {
        printf("Error with dir opening\n");
        return -1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
            continue;
        if (i >= MAXFILES) {
            printf("Too many files (limit: %d)\n", MAXFILES);
            break;
        }
        strncpy(files_mas[i], entry->d_name, MAXLINE - 1);
        files_mas[i][MAXLINE - 1] = '\0';
        i++;
    }
    closedir(dir);
    return i;
}

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