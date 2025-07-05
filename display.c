#include "display.h"
#include "input.h"
#include "foreground.h"
#include "log.h"
#include "globals.h"

void display_username_and_system()
{
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("gethostname");
        exit(1);
    }

    char *dot = strstr(hostname, ".local");
    if (dot != NULL)
        *dot = '\0';

    uid_t uid = getuid();
    struct passwd *p = getpwuid(uid);
    if (p == NULL)
    {
        perror("getpwuid");
        exit(1);
    }

    snprintf(username, sizeof(username), "%s", p->pw_name);

    if (getcwd(current_directory, sizeof(current_directory)) == NULL)
    {
        perror("getcwd");
        exit(1);
    }
    // printf("%s",home);
    int home_len = strlen(shell_home_directory);
    int curr_len = strlen(current_directory);

    if (curr_len == home_len)
    {
        if (strcmp(shell_home_directory, current_directory) == 0)
            snprintf(current_directory, sizeof(current_directory), "~");
    }
    else if (curr_len > home_len)
    {
        if (memcmp(current_directory, shell_home_directory, home_len) == 0 && current_directory[home_len] == '/')
            snprintf(current_directory, sizeof(current_directory), "~%s", current_directory + home_len);
    }
    if(time_flag == 0)
    printf("<%s@%s:%s> ", username, hostname, current_directory);
    else
    printf("<%s@%s:%s %s> ", username, hostname, current_directory,process_name);
    

    fflush(stdout);
}

int main()
{
    if (getcwd(shell_home_directory, sizeof(shell_home_directory)) == NULL)
    {
        perror("getcwd");
    }
    snprintf(logpath,sizeof(logpath),"%s/log_file.txt",shell_home_directory);

    setup_sigchld_handler();
    load_log();

    while (1)
    {

        check_background_processes();
        display_username_and_system();
        flag =0;
        time_flag = 0;

        if (fgets(INPUT_MAIN, sizeof(INPUT_MAIN), stdin) == NULL)
        {
            printf("\n");
            break;
        }
        // return 0;
        // printf("%s\n",INPUT_MAIN);
        long long len = strlen(INPUT_MAIN);
        if (len > 0 && INPUT_MAIN[len - 1] == '\n')
        {
            INPUT_MAIN[len - 1] = '\0';
        }

        char input[MAX_INPUT];
        strcpy(input, INPUT_MAIN);

        check_commands(input);

        if (flag == 0)
            add_to_log(INPUT_MAIN);

    }
}


