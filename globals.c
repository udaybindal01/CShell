#include "globals.h"


char INPUT_MAIN[MAX_INPUT];
char shell_home_directory[PATH_MAX];
char username[100000];
char hostname[100000];
char current_directory[100000];
char previous_dir[PATH_MAX] = "";
char process_name[4096];
char logpath[8192];

int time_flag = 0;
int flag =0;
int saved_stdouts =0;
int saved_stdins = 0;
// struct ProcessStatus Pro[1000000];

int is_foreground_process = 0;
int fgpid =-1;
int fl=0;
int shell_pid = 0;

char store[4096];

