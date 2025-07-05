#include "display.h"
#include "input.h"
#include "foreground.h"
#include "log.h"
#include "myshrc.h"
#include "inout.h"
#include "signal.h"
#include "fgandbg.h"
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
    if (time_flag == 0)
        printf("<%s@%s:%s> ", username, hostname, current_directory);
    else
        printf("<%s@%s:%s %s> ", username, hostname, current_directory, process_name);

    fflush(stdout);
}

int main()
{
    shell_pid = getpid();
    if (setpgid(shell_pid, shell_pid) < 0)
    {
        perror("Couldn't put the shell in its own process group");
        exit(1);
    }

    if (getcwd(shell_home_directory, sizeof(shell_home_directory)) == NULL)
    {
        perror("getcwd");
    }
    char str[1000];
    strcpy(str, "log_file.txt");
    snprintf(logpath, sizeof(logpath), "%s/%s", shell_home_directory, str);
    setup_sigchld_handler();
    load_log();
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigstp);
    // load_file();
    load_myshrc();
    saved_stdins = dup(STDIN_FILENO);
    saved_stdouts = dup(STDOUT_FILENO);
    
    pid_t pid;
    while (1)
    {
        fgpid =0;
        // printf("%d\n",fgpid);
        check_background_processes();
        display_username_and_system();
        flag = 0;
        time_flag = 0;
    
        if (fgets(INPUT_MAIN, sizeof(INPUT_MAIN), stdin) == NULL)
        {
            printf("\n");
            handle_ctrl_d();
            break;
        }
            
        long long len = strlen(INPUT_MAIN);
        if (len > 0 && INPUT_MAIN[len - 1] == '\n')
        {
            INPUT_MAIN[len - 1] = '\0';
        }

        char input[MAX_INPUT];
        // strcpy(input, resolve_alias(INPUT_MAIN));

        if (sscanf(INPUT_MAIN, "fg %d", &pid) == 1)
            fg_change(pid);
        else if (sscanf(INPUT_MAIN, "bg %d", &pid) == 1)
            bg_change(pid);
        else
        {
            // printf("MULTIPLE\n");
            check_commands(INPUT_MAIN);
        }
        // parse_and_execute(input);

        // dup2(saved_stdin, STDIN_FILENO);
        // dup2(saved_stdout, STDOUT_FILENO);

        if (flag == 0)
            add_to_log(INPUT_MAIN);
    }
    clearerr(stdin);
    close(saved_stdins);
    close(saved_stdouts);
}
