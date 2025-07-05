
#ifndef MY_H
#define MY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_ALIAS 100
#define MAX_FUNCTION 100
#define MAX_COMMAND 1000
#define MAX_FUNCTION_BODY 5000

struct Alias
{
    char name[50];
    char command[100];
};

struct Function
{
    char name[50];
    char body[MAX_FUNCTION_BODY];
};

extern struct Alias aliases[MAX_ALIAS];
extern struct Function functions[MAX_FUNCTION];
extern int alias_count;
extern int function_count;

void load_myshrc();
void execute_function(const char *function_name);
void trim(char *str);

#endif
