#ifndef REVEAL_H
#define REVEAL_H

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pwd.h>
#include <limits.h>
#include <grp.h>
#include <time.h>


void reveal(char* command);
int get_details(char *path);
void print_details(char* name, char* path);
void print_extended(struct stat *file, char *color, char *name);
int compare_entries(const void *a, const void *b);

#endif