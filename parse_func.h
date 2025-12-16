#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checks.h"

#define MAXLINE 1000
#define MAX_STYLES 64
#define MAX_EXT_LEN 16
#define MAX_COMM_LEN 8

void edit_file(size_t max_len, char *filename, int fixed);
void create_edited_file(size_t max_len, char *filename, int fixed);
void remove_old_comms(char *filename);
size_t file_parse(char *filename);
char *add_aligned(char *filename, char *string_to_add);
char *parse_extension(const char *filename);

typedef struct {
    char ext[MAX_EXT_LEN];
    char comm[MAX_COMM_LEN];
} Style;

extern Style styles[MAX_STYLES];
extern int style_count;
extern const char ext_filename[];

#endif