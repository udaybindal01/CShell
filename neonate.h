#ifndef NEO_H
#define NEO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>



void set_terminal_mode();
void reset_terminal_mode();
void get_latest_pid(int *latest_pid);
void neonate(char *command);
void neo2();

#endif