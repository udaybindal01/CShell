#include "display.h"
#include "input.h"
#include "foreground.h"
#include "log.h"
#include "activity.h"
#include "fgandbg.h"
#include "signal.h"
#include "globals.h"

int g = 0;

int bg_count = 0;
BgProcess bg[10000];
struct ProcessStatus Pro[1000000];

void handle_sigchld(int sig)
{
    // printf("nyan %d\n",sig);
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
    struct sigaction sa_chld;
    sa_chld.sa_handler = handle_sigchld;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1)
    {
        perror("sigaction SIGCHLD");
        exit(1);
    }
    // struct sigaction sa_int;
    // sa_int.sa_handler = handle_sigint;
    // sigemptyset(&sa_int.sa_mask);
    // sa_int.sa_flags = 0;

    // if (sigaction(SIGINT, &sa_int, NULL) == -1)
    // {
    //     perror("sigaction SIGINT");
    //     exit(1);
    // }

    // struct sigaction sa_tstp;
    // sa_tstp.sa_handler = handle_sigstp;
    // sigemptyset(&sa_tstp.sa_mask);
    // sa_tstp.sa_flags = 0;

    // if (sigaction(SIGTSTP, &sa_tstp, NULL) == -1)
    // {
    //     perror("sigaction SIGTSTP");
    //     exit(1);
    // }
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
        if (flag == 1) setpgid(0, 0);

        char *args[] = {"/bin/bash", "-c", command, NULL};
        execvp(args[0], args);
        perror("execvp");
    }
    else
    {
        fgpid = p;
        Pro[fgpid].pid = fgpid;
        g = p;
        char *redir_pos = strpbrk(command, "><");
        if (redir_pos != NULL)
        {
            *redir_pos = '\0';
        }

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
            // printf("%d\n", p);
            // get_process_status(fgpid);

            Pro[fgpid].status = 0;
            is_foreground_process = 1;
            int status;
            start = time(NULL);
            waitpid(p, &status, 0);
            end = time(NULL);
            int rounded_time = (int)(end - start);
            if (rounded_time > 2)
            {
                snprintf(process_name, sizeof(process_name), "%s : %ds", command, rounded_time);
                time_flag = 1;
            }
            else
            {
                time_flag = 0;
            }
            is_foreground_process = 0;
            add_process_to_queue(p, command);
        }
        else
        {

            // setpgid(0, 0);
            Pro[fgpid].status = 1;
            Pro[0].status = 1;
            int status;
            if (waitpid(p, &status, WNOHANG) == -1)
            {
                printf("invalid command\n");
                return;
            }
            setpgid(p, p);
            add_process_to_queue(p, command);
            add_bg_process(p, command);
            printf("%d\n", p);
            bg[bg_count].pid = p;
            strcpy(bg[bg_count].name, process_name);
            bg_count++;
        }
    }
}