#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pwd.h>
#include <limits.h>

void check_commands(char *input);
void execute_command(char *command, int background);
void get_dir(char *dir, int flag);


#endif