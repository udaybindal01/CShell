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
void get_dir(char *dir, int flag);
char *trim_whitespace(char *str);
void execute_pipes_redirection(char **commands, int num_commands,char *input_file, char *output_file,int append_mode);
void execute_command(char *command, int background, int input_fd, int output_fd);


#endif