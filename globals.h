#include <limits.h>

#undef MAX_INPUT
#define MAX_INPUT 1024
#undef PATH_MAX
#define PATH_MAX 4096

extern char INPUT_MAIN[MAX_INPUT];
extern char shell_home_directory[PATH_MAX];
extern char username[100000];
extern char hostname[100000];
extern char current_directory[100000];
extern char previous_dir[PATH_MAX];
extern char process_name[4096];
extern char logpath[4096];

extern int time_flag;
extern int flag;

extern char store[4096];
