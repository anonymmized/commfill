#ifndef CHECKS_H
#define CHECKS_H
#define MAXLINE 1000

void get_slash(char *filename_sl, char *filename);
int get_filename(char *line, int lim);
int check_filename(char *filename);
int filenames_init(char files_mas[][MAXLINE], char *dir);

#endif