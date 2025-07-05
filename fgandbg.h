#ifndef FDNBG_H
#define FGNBG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <sys/wait.h>
#include <termios.h> 
#include <unistd.h>

typedef struct
{

    pid_t pid;
    char name[256];
    int is_stopped;
} Processes;

extern Processes bg_processes[100];
extern int bg_count_process ;

void add_bg_process(pid_t pid,char *name);
void bg_change(pid_t pid);
void fg_change(pid_t pid);
void remove_from_bg_queue(pid_t pid);
void mark_process_stopped(pid_t pid);


#endif
