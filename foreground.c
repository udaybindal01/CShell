#include "display.h"
#include "input.h"
#include "foreground.h"
#include "log.h"

int bg_count = 0;
typedef struct background_process
{
    pid_t pid;
    char name[100];
} BgProcess;

BgProcess bg[10000];

int is_foreground_process = 0;

void handle_sigchld(int sig)
{
    int status;
    pid_t pid;

    if (is_foreground_process)
        return;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        for (int i = 0; i < bg_count; ++i)
        {
            if (bg[i].pid == pid)
            {

                char st[100];
                if (WIFEXITED(status))
                    strcpy(st, "normally");
                else
                    strcpy(st, "abnormally");

                if (strcmp(bg[i].name, "sleep") == 0)
                    printf("\n%s exited %s (%d)\n", bg[i].name, st, bg[i].pid);
                else
                    printf("%s exited %s (%d)\n", bg[i].name, st, bg[i].pid);

                fflush(stdout);
                for (int j = i; j < bg_count - 1; ++j)
                {
                    bg[j] = bg[j + 1];
                }
                --bg_count;
                --i;

            }
        }

        display_username_and_system();
        fflush(stdout);
    }
}

void setup_sigchld_handler()
{
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        // _exit(1);
    }
}

void check_background_processes()
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        for (int i = 0; i < bg_count; ++i)
        {
            if (bg[i].pid == pid)
            {
                char st[100];
                if (WIFEXITED(status))
                    strcpy(st, "normally");
                else
                    strcpy(st, "abnormally");
                if (strcmp(bg[i].name, "sleep") == 0)
                    printf("\n%s exited %s (%d)\n", bg[i].name, st, bg[i].pid);
                else
                    printf("%s exited %s (%d)\n", bg[i].name, st, bg[i].pid);

                for (int j = i; j < bg_count - 1; ++j)
                {
                    bg[j] = bg[j + 1];
                }
                --bg_count;
                --i;
                fflush(stdout);
            }
        }
    }
}

void foreg(char *command, int flag)
{
    pid_t p = fork();
    time_t start, end;

    if (p < 0)
    {
        perror("fork fail");
        // exit(1);
    }
    else if (p == 0)
    {
        char *args[] = {"sh", "-c", command, NULL};
        execvp(args[0], args);
        perror("Failed to execute execvp");
    }
    else
    {
        int i = 0;
        while (isspace(command[i]))
            i++;

        int j = 0;
        while (command[i] != '\0' && !isdigit(command[i]) && command[i] != ' ')
        {
            process_name[j++] = command[i++];
        }
        process_name[j] = '\0';

        if (flag == 0)
        {
            is_foreground_process = 1;
            int status;
            start = time(NULL);
            waitpid(p, &status, 0);
            end = time(NULL);
            // if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            // {
            //     add_to_log(store);
            // }
            // add_to_log(store);
            int rounded_time = (int)(end - start);
            if (rounded_time > 2)
            {
                // printf("<%s@%s:%s %s : %ds> ", username, hostname, current_directory, process_name, rounded_time);
                snprintf(process_name, sizeof(process_name), "%s : %ds", command, rounded_time);
                time_flag = 1;
            }
            else
            {
                time_flag = 0;
            }
            is_foreground_process = 0;
        }
        else
        {
            int status;
            if (waitpid(p, &status, WNOHANG) == -1)
            {
                printf("invalid command\n");
                return;
            }
            printf("%d\n", p);
            bg[bg_count].pid = p;
            strcpy(bg[bg_count].name, process_name);
            // add_to_log(store);
            bg_count++;
        }
    }
}
