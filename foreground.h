#ifndef FOREG_H
#define FOREG_H

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

extern int bg_count;
typedef struct background_process
{
    pid_t pid;
    char name[100];
} BgProcess;

extern BgProcess bg[10000];

struct ProcessStatus {
    int pid;  
    int status;  
};

extern struct ProcessStatus Pro[1000000];

void foreg(char* command,int flag);
void setup_sigchld_handler();
void handle_sigchld(int sig);
void check_background_processes();

extern int g ;


#endif