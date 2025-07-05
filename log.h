#ifndef LOG_H
#define LOG_H

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

void logs(char *command);
void log_execute(int n);
void add_to_log(char* command);
void log_purge();
void show_log();
void load_log();

#endif