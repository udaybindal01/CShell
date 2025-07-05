#ifndef HOP_H
#define HOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pwd.h>
#include <limits.h>

#define MAX_INPUT 1024

void hop(char *command);
int execute_hop(char *dir);

#endif