#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

void edit_file(size_t max_len, char *filename);
void get_slash(char *filename_sl, char *filename);
int get_filename(char *line, int lim);
int check_filename(char *filename);
size_t file_parse(char *filename);

void edit_file(size_t max_len, char *filename) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        printf("Error: cannot open file for reading.\n");
        return;
    }

    char temp_filename[MAXLINE];
    snprintf(temp_filename, sizeof(temp_filename), "%s.tmp", filename);

    FILE *output = fopen(temp_filename, "w");
    if (!output) {
        printf("Error: cannot create temporary file.\n");
        fclose(input);
        return;
    }

    char line[MAXLINE];
    while (fgets(line, sizeof(line), input)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }

        fputs(line, output);
        for (size_t i = len; i < max_len; i++) {
            fputc(' ', output);
        }
        fputs(" //\n", output);
    }
    fclose(input);
    fclose(output);

    if (remove(filename) != 0) {
        printf("Warning: could not delete original file.\n");
    } 
    if (rename(temp_filename, filename) != 0) {
        printf("Error: could not rename temporary file.\n");
    }
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

size_t file_parse(char *filename) {
    FILE *fn = fopen(filename, "r");
    if (!fn) {
        printf("Error with opening file.\n");
        return 0;
    }
    char buffer[MAXLINE];
    size_t max_len = 0;
    while (fgets(buffer, sizeof(buffer), fn)) {
        size_t len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n') {
            len--;
        }

        if (len == MAXLINE - 1 && buffer[len - 1] != '\n') {
            int c;
            while ((c = fgetc(fn)) != EOF && c != '\n') {
                len++;
            }
        }

        if (len > max_len) {
            max_len = len;
        }
    }
    fclose(fn);
    return max_len;
}

int main() {
    char filename[MAXLINE];
    char filename_sl[MAXLINE];
    int len = get_filename(filename, MAXLINE);
    if (len == 0) {
        printf("No filename entered.\n");
        return 1;
    }

    get_slash(filename_sl, filename);
    printf("Filename: %s\n", filename_sl);

    if (check_filename(filename_sl)) {
        printf("Warning: filename has no extension.\n");
    }
    
    size_t max_len = file_parse(filename);
    printf("Max line length: %zu\n", max_len);

    if (max_len > 0) {
        edit_file(max_len, filename);
        printf("File updated successfully.\n");
    } else {
        printf("File is empty or could not be processed.\n");
    }
    return 0;
}