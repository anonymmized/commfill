#include "parse_func.h"

Style styles[MAX_STYLES];
int style_count = 0;
const char ext_filename[] = "comment_styles.txt";

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