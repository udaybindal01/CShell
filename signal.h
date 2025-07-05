#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>


void handle_ping(int num, pid_t pid);
void handle_sigint(int sig) ;
void handle_ctrl_d() ;
void handle_sigstp(int sig) ;
void ping(char* command);
void get_process_status(pid_t pid);
#endif
