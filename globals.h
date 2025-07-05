#include <limits.h>

#undef MAX_INPUT
#define MAX_INPUT 1024
#undef PATH_MAX
#define PATH_MAX 4096

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
#include <fcntl.h>  

extern char INPUT_MAIN[MAX_INPUT];
extern char shell_home_directory[PATH_MAX];
extern char username[100000];
extern char hostname[100000];
extern char current_directory[100000];
extern char previous_dir[PATH_MAX];
extern char process_name[4096];
extern char logpath[8192];

extern int time_flag;
extern int flag;
extern int saved_stdins;
extern int saved_stdouts;

extern int is_foreground_process ;
extern int fgpid;
extern int fl;
extern int shell_pid;


extern char store[4096];
