#include <stdio.h>                                                                              //
#include <stdlib.h>                                                                             //
#include <string.h>                                                                             //
                                                                                                //
#define MAXLINE 1000                                                                            //
                                                                                                //
void edit_file(size_t max_len, char *filename);                                                 //
void remove_old_comms(char *filename);                                                          //
void get_slash(char *filename_sl, char *filename);                                              //
int get_filename(char *line, int lim);                                                          //
int check_filename(char *filename);                                                             //
size_t file_parse(char *filename);                                                              //
                                                                                                //
void remove_old_comms(char *filename) {                                                         //
    FILE *input = fopen(filename, "r");                                                         //
    if (!input) {                                                                               //
        printf("Error: cannot open file for reading.\n");                                       //
        return;                                                                                 //
    }                                                                                           //
                                                                                                //
    char temp_filename[MAXLINE];                                                                //
    snprintf(temp_filename, sizeof(temp_filename), "%s.tmp", filename);                         //
                                                                                                //
    FILE *output = fopen(temp_filename, "w");                                                   //
    if (!output) {                                                                              //
        printf("Error: cannot create temporary file.\n");                                       //
        fclose(input);                                                                          //
        return;                                                                                 //
    }                                                                                           //
                                                                                                //
    char line[MAXLINE];                                                                         //
    while (fgets(line, sizeof(line), input)) {                                                  //
        int in_string = 0;                                                                      //
        int escaped = 0;                                                                        //
        char *comment_start = NULL;                                                             //
        for (char *p = line; *p != '\0'; p++) {                                                 //
            if (!escaped) {                                                                     //
                if (*p == '"') {                                        //                // // //
                    in_string = !in_string;                                                     //
                }                                                                               //
                if (*p == '/' && *(p + 1) == '/' && !in_string) {                               //
                    comment_start = p;                                                          //
                }                                                                               //
            }                                                                                   //
            escaped = (!escaped && *p == '\\');                                                 //
        }                                                                                       //
        if (comment_start) {                                                                    //
            *comment_start = '\n';                                                              //
            *(comment_start + 1) = '\0';                                                        //
        }                                                                                       //
        fputs(line, output);                                                                    //
    }                                                                                           //
    fclose(input);                                                                              //
    fclose(output);                                                                             //
                                                                                                //
    remove(filename);                                                                           //
    rename(temp_filename, filename);                                                            //
}                                                                                               //
                                                                                                //
void edit_file(size_t max_len, char *filename) {                                                //
    FILE *input = fopen(filename, "r");                                                         //
    if (!input) {                                                                               //
        printf("Error: cannot open file for reading.\n");                                       //
        return;                                                                                 //
    }                                                                                           //
                                                                                                //
    char temp_filename[MAXLINE];                                                                //
    snprintf(temp_filename, sizeof(temp_filename), "%s.tmp", filename);                         //
                                                                                                //
    FILE *output = fopen(temp_filename, "w");                                                   //
    if (!output) {                                                                              //
        printf("Error: cannot create temporary file.\n");                                       //
        fclose(input);                                                                          //
        return;                                                                                 //
    }                                                                                           //
                                                                                                //
    char line[MAXLINE];                                                                         //
    while (fgets(line, sizeof(line), input)) {                                                  //
        size_t len = strlen(line);                                                              //
        if (len > 0 && line[len - 1] == '\n') {                                                 //
            line[len - 1] = '\0';                                                               //
            len--;                                                                              //
        }                                                                                       //
                                                                                                //
        fputs(line, output);                                                                    //
        for (size_t i = len; i < max_len; i++) {                                                //
            fputc(' ', output);                                                                 //
        }                                                                                       //
        fputs(" //\n", output);                                                                 //
    }                                                                                           //
    fclose(input);                                                                              //
    fclose(output);                                                                             //
                                                                                                //
    if (remove(filename) != 0) {                                                                //
        printf("Warning: could not delete original file.\n");                                   //
    }                                                                                           //
    if (rename(temp_filename, filename) != 0) {                                                 //
        printf("Error: could not rename temporary file.\n");                                    //
    }                                                                                           //
}                                                                                               //
                                                                                                //
void get_slash(char *filename_sl, char *filename) {                                             //
    char *last_slash = strrchr(filename, '/');                                                  //
    if (last_slash) {                                                                           //
        strcpy(filename_sl, last_slash + 1);                                                    //
    } else {                                                                                    //
        strcpy(filename_sl, filename);                                                          //
    }                                                                                           //
}                                                                                               //
                                                                                                //
int get_filename(char *line, int lim) {                                                         //
    printf("Enter the filename (with extension): ");                                            //
    if (fgets(line, lim, stdin) == NULL) {                                                      //
        line[0] = '\0';                                                                         //
        return 0;                                                                               //
    }                                                                                           //
    size_t len = strlen(line);                                                                  //
    if (len > 0 && line[len - 1] == '\n') {                                                     //
        line[len - 1] = '\0';                                                                   //
    }                                                                                           //
    return (int)len;                                                                            //
}                                                                                               //
                                                                                                //
int check_filename(char *filename) {                                                            //
    return (strchr(filename, '.') == NULL);                                                     //
}                                                                                               //
                                                                                                //
size_t file_parse(char *filename) {                                                             //
    FILE *fn = fopen(filename, "r");                                                            //
    if (!fn) {                                                                                  //
        printf("Error with opening file.\n");                                                   //
        return 0;                                                                               //
    }                                                                                           //
    char buffer[MAXLINE];                                                                       //
    size_t max_len = 0;                                                                         //
    while (fgets(buffer, sizeof(buffer), fn)) {                                                 //
        size_t len = strlen(buffer);                                                            //
                                                                                                //
        if (len > 0 && buffer[len - 1] == '\n') {                                               //
            len--;                                                                              //
        }                                                                                       //
                                                                                                //
        if (len == MAXLINE - 1 && buffer[len - 1] != '\n') {                                    //
            int c;                                                                              //
            while ((c = fgetc(fn)) != EOF && c != '\n') {                                       //
                len++;                                                                          //
            }                                                                                   //
        }                                                                                       //
                                                                                                //
        if (len > max_len) {                                                                    //
            max_len = len;                                                                      //
        }                                                                                       //
    }                                                                                           //
    fclose(fn);                                                                                 //
    return max_len;                                                                             //
}                                                                                               //
                                                                                                //
int main(int argc, char *argv[]) {                                                              //
    char filename[MAXLINE];                                                                     //
    int remove_old = 0;                                                                         //
    int file_input = 0;                                                                         //
    int get_name = 0;                                                                           //
    char *file_input_s = NULL;                                                                  //
    char **arg = argv + 1;                                                                      //
    int remaining = argc - 1;                                                                   //
    while (remaining > 0) {                                                                     //
        char *current = *arg;                                                                   //
        if (current[0] == '-' && current[1] != '\0') {                                          //
            if (strcmp(current, "-i") == 0) {                                                   //
                if (remaining > 1) {                                                            //
                    file_input = 1;                                                             //
                    file_input_s = *(arg + 1);                                                  //
                    arg += 2;                                                                   //
                    remaining -= 2;                                                             //
                    continue;                                                                   //
                } else {                                                                        //
                    printf("Error: option -i requires a filename\n");                           //
                    return 1;                                                                   //
                }                                                                               //
            } else if (strcmp(current, "-r") == 0) {                                            //
                remove_old = 1;                                                                 //
                arg++;                                                                          //
                remaining--;                                                                    //
                continue;                                                                       //
            } else {                                                                            //
                printf("Unknown option: %s\n", current);                                        //
            }                                                                                   //
        } else {                                                                                //
            break;                                                                              //
        }                                                                                       //
    }                                                                                           //
    char filename_sl[MAXLINE];                                                                  //
    if (file_input == 1) {                                                                      //
        for (int i = 0; i < strlen(file_input_s); i++) {                                        //
            filename[i] = file_input_s[i];                                                      //
        }                                                                                       //
        get_name = 1;                                                                           //
    }                                                                                           //
    int len = get_filename(filename, MAXLINE);                                                  //
    if (len == 0 && get_name == 0) {                                                            //
        printf("No filename entered.\n");                                                       //
        return 1;                                                                               //
    }                                                                                           //
                                                                                                //
    get_slash(filename_sl, filename);                                                           //
    printf("Filename: %s\n", filename_sl);                                                      //
                                                                                                //
    if (check_filename(filename_sl)) {                                                          //
        printf("Warning: filename has no extension.\n");                                        //
    }                                                                                           //
    if (remove_old == 1) {                                                                      //
        remove_old_comms(filename);                                                             //
        printf("Old comments were deleted\n");                                                  //
        return 0;                                                                               //
    }                                                                                           //
    size_t max_len = file_parse(filename);                                                      //
    printf("Max line length: %zu\n", max_len);                                                  //
                                                                                                //
    if (max_len > 0) {                                                                          //
        edit_file(max_len, filename);                                                           //
        printf("File updated successfully.\n");                                                 //
    } else {                                                                                    //
        printf("File is empty or could not be processed.\n");                                   //
    }                                                                                           //
                                                                                                //
    return 0;                                                                                   //
}                                                                                               //
