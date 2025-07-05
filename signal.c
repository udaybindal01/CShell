#include "globals.h"
#include "input.h"
#include "signal.h"
#include "activity.h"
#include "foreground.h"
#include "display.h"
#include "fgandbg.h"

void handle_ping(int num, pid_t pid)
{
    int signal_num = num % 32;
    if (kill(pid, 0) == -1)
    {
        if (errno == ESRCH)
        {
            printf("No such process found\n");
        }
        else
        {
            perror("Error checking process");
        }
        return;
    }

    if (kill(pid, signal_num) == 0)
    {
        printf("Sent signal %d to process with pid %d\n", signal_num, pid);
    }
    else
    {
        perror("Error sending signal");
    }
}

void handle_sigint(int sig)
{
    // printf("%d %d\n",fgpid, Pro[fgpid].status);
    if (fgpid == -1)
    {
        if (kill(fgpid, 0) != 0)
        {
            perror("kill");
            return;
        }
        return;
    }
    else if (Pro[fgpid].status == 0)
    {
        // printf("HERE\n");

        if (kill(fgpid, SIGINT) != 0)
        {
            perror("kill");
            return;
        }
        // fgpid = 0;
        return;
    }
}

void handle_ctrl_d()
{

    for (int i = 0; i < process_count; i++)
    {
        if (kill(process_queue[i].pid, 0) == 0)
        {
            kill(process_queue[i].pid, SIGTERM);
        }
    }

    if (feof(stdin))
    {
        exit(0);
    }
}

void get_process_status(pid_t pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening status file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "State:", 6) == 0)
        {
            printf("%s", line);
        }
    }

    fclose(file);
}

void handle_sigstp(int sig)
{
    printf("%d\n", fgpid);
    // get_process_status(fgpid);
    if (fgpid > 0)
    {

        // kill(fgpid, SIGSTOP);
        // int status;
        // if (waitpid(fgpid, &status, WNOHANG) == -1)
        // {
        //     printf("invalid command\n");
        //     return;
        // }
        kill(fgpid, 20);
        // get_process_status(fgpid);
        printf("\nProcess with PID %d stopped\n", fgpid);
        // add_bg_process(p, command);
        bg_processes[bg_count_process].pid = fgpid;
        bg_count_process++;
        display_username_and_system();
        fflush(stdout);
        fgpid = 0;
        return;
    }
    else
    {
        printf("No foreground process running\n");
        display_username_and_system();
        fflush(stdout);
        return;
    }
}

void ping(char *command)
{

    char *redir_pos = strpbrk(command, "><");
    if (redir_pos != NULL)
    {
        *redir_pos = '\0';
    }

    const char *delimiter = " ";
    char *saveptr;
    char *args = strtok_r(command, delimiter, &saveptr);
    args = strtok_r(NULL, delimiter, &saveptr);
    pid_t p;
    int num;

    char *start = args;
    while (isspace((unsigned char)*start))
        start += 1;
    char *end = start + strlen(start) - 1;
    while (isspace((unsigned char)*end) && end > start)
        end--;
    *(end + 1) = '\0';

    p = atoi(start);

    args = strtok_r(NULL, delimiter, &saveptr);

    start = args;
    while (isspace((unsigned char)*start))
        start += 1;
    end = start + strlen(start) - 1;
    while (isspace((unsigned char)*end) && end > start)
        end--;
    *(end + 1) = '\0';

    num = atoi(start);
    handle_ping(num, p);
}
