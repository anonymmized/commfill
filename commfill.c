#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_func.h"
/*
#define MAXLINE 1000
#define MAX_STYLES 64
#define MAX_EXT_LEN 16
#define MAX_COMM_LEN 8
*/
/*
typedef struct {
    char ext[MAX_EXT_LEN];
    char comm[MAX_COMM_LEN];
} Style;
*/
// Style styles[MAX_STYLES];
/*
int style_count = 0;
const char ext_filename[] = "comment_styles.txt";
*/

// void edit_file(size_t max_len, char *filename, int fixed);
void create_edited_file(size_t max_len, char *filename, int fixed);
// void remove_old_comms(char *filename);
void get_slash(char *filename_sl, char *filename);
int get_filename(char *line, int lim);
int check_filename(char *filename);
// size_t file_parse(char *filename);
// char *add_aligned(char *filename, char *string_to_add);
// char *parse_extension(const char *filename);

/*
char *parse_extension(const char *filename) {
    FILE *fp = fopen(ext_filename, "r");
    if (!fp) return NULL;

    char line[MAXLINE];
    style_count = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        if (style_count < MAX_STYLES) {
            if (sscanf(line, "%15s %7s", styles[style_count].ext, styles[style_count].comm) == 2) {
                style_count += 1;
            }
        }
    }
    fclose(fp);

    const char *dot = strrchr(filename, '.');
    if (!dot) return NULL;
    for (int i = 0; i < style_count; i++) {
        if (strcmp(dot, styles[i].ext) == 0) {
            char *result = malloc(MAX_COMM_LEN);
            if (result) {
                strncpy(result, styles[i].comm, MAX_COMM_LEN - 1);
                result[MAX_COMM_LEN - 1] = '\0';
            }
            return result;
        }
    }
    return NULL;
}
*/
/*
char *add_aligned(char *filename, char *string_to_add) {
    int len_filename = strlen(filename);
    int len_string = strlen(string_to_add);
    char* new_filename = malloc(MAXLINE * sizeof(char));
    int i = 0;
    int j;
    for (j = 0; j < len_filename - 1 && filename[j] != '.'; j++)
        new_filename[i++] = filename[j];
    for (int k = 0; k < len_string; k++) {
        new_filename[i++] = string_to_add[k];
    }
    while (filename[j] != '\0') {
        new_filename[i++] = filename[j++];
    }
    new_filename[i] = '\0';
    return new_filename;
}
*/
/*
void remove_old_comms(char *filename) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        printf("Error: cannot open file for reading.\n");
        return;
    }

    char *comment_prefix = parse_extension(filename);
    if (!comment_prefix) {
        printf("Warning: unknown file type, skipping comment removal.\n");
        fclose(input);
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
        int in_string = 0;
        int escaped = 0;
        char *comment_start = NULL;
        size_t comm_len = strlen(comment_prefix);
        char *p = line;
        while (*p) {
            if (!escaped && *p == '"') {
                in_string = !in_string;
            }

            if (!in_string && strncmp(p, comment_prefix, comm_len) == 0) {
                char *trim_pos = p;
                while (trim_pos > line && (*(trim_pos - 1) == ' ' || *(trim_pos - 1) == '\t')) trim_pos--;
                *trim_pos = '\n';
                *(trim_pos + 1) = '\0';
                break;
            }
            escaped = (!escaped && *p == '\\');
            p++;
        }
        fputs(line, output);
    }
    fclose(input);
    fclose(output);

    remove(filename);
    rename(temp_filename, filename);

    free(comment_prefix);
}
*/

void create_edited_file(size_t max_len, char *filename, int fixed) {
    char string_to_add[] = "_aligned";
    FILE *input = fopen(filename, "r");
    if (!input) {
        printf("Error: cannot open file for reading.\n");
        return;
    }

    char *comment_prefix = parse_extension(filename);
    char *temp_filename = add_aligned(filename, string_to_add);
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
        if (fixed > 0) {
            for (size_t i = 0; i < fixed; i++) {
                fputc(' ', output);
            }
        } else {
            for (size_t i = len; i < max_len; i++) {
                fputc(' ', output);
            }
        }
        fprintf(output, " %s\n", comment_prefix);
    }
    fclose(input);
    fclose(output);

}
/*
void edit_file(size_t max_len, char *filename, int fixed) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        printf("Error: cannot open file for reading.\n");
        return;
    }
    char *comment_prefix = parse_extension(filename);
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
        if (fixed > 0) {
            for (size_t i = 0; i < fixed; i++) {
                fputc(' ', output);
            }
        } else {
            for (size_t i = len; i < max_len; i++) {
                fputc(' ', output);
            }
        }
        fprintf(output, " %s\n", comment_prefix);
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
*/
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
/*
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
*/

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