#ifndef ACT_H
#define ACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>

typedef struct {
    pid_t pid;
    char command[1024];
    int is_running; 
} Process;

extern Process process_queue[1024];

extern int process_count;

const char* get_state_description(char state) ;
int read_process(const char *pid, char *state);
int is_numeric(const char *str) ;
void activities();
void add_process_to_queue(pid_t pid, const char *command) ;

#endif
