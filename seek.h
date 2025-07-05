#ifndef SEEK_H
#define SEEK_H

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
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>

void seek(char *command);
void seek_directory(const char *target, const char *dir);
#endif