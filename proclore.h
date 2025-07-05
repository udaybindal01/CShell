#ifndef PRO_H
#define PRO_H

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
#include <dirent.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <linux/kernel.h>

void pro(char* command);
void exe_pro(pid_t pid);



#endif