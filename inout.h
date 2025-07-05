#ifndef INOUT_H
#define INOUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

void parse_and_execute(char *input, int flag) ;
// void handle_pipers(char* command);
void handle_redirection(char* command, int flag);
#endif
