#ifndef iMAN_H
#define iMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUFFER_SIZE 4096

void iMans(const char *command);
void parseiMan(char* command);
void decode(char *input, int l);
char* cut(const char* input) ;


#endif