#include "globals.h"
#include "fgandbg.h"
#include "foreground.h"


Processes bg_processes[100];
int bg_count_process =0;

void fg_change(pid_t pid)
{
    int found = 0;
    pid_t shell_pgid = getpgrp(); 

    
    // for (int i = 0; i < bg_count_process; i++)
    // {
    //     printf("%d %d\n", bg_processes[i].pid, bg_processes[i].is_stopped);
    // }

    for (int i = 0; i < bg_count_process; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            found = 1;

            
            for (int j = i + 1; j < bg_count_process; j++)
            {
                bg_processes[j - 1] = bg_processes[j];
            }
            bg_count_process--; 

            if (kill(pid, 0) == -1)
            {
                perror("Error checking process");
                return;
            }

            // if (setpgid(pid, pid) < 0)
            // {
            //     perror("Couldn't put the process in its own group");
            //     return;
            // }

            //  if (tcsetpgrp(STDIN_FILENO, pid) < 0)
            // {
            //     perror("Couldn't give the terminal to the process");
            //     return;
            // }
            // if (tcsetpgrp(shell_fd, process_gid) == -1) {
            //     perror("tcsetpgrp (bring process to foreground) failed");
            //     close(shell_fd);
            //     return;
            // }
            // fgpid=pid;
            if (kill(pid, SIGCONT) < 0)
            {
                perror("Couldn't send SIGCONT to process group");
                return;
            }
            // fgpid = 0;
            printf("Process [%d] brought to foreground.\n", pid);
            setpgid(g, g);
            fgpid = g;
            Pro[fgpid].status = 0;
            int status;
            if (waitpid(pid, &status, WUNTRACED) == -1) {
                perror("waitpid failed");
            }
            // if (tcsetpgrp(shell_fd,shell_pgid) == -1) {
            //     perror("tcsetpgrp (restore shell control) failed");
            // }
            // if (tcsetpgrp(STDIN_FILENO, shell_pid) < 0)
            // {
            //     perror("Couldn't put shell back in foreground");
            // }

            // close(shell_fd);
            // kill(pid, SIGCONT);
            // // tcsetpgrp(shell_pid, pid);
            // int status;
            // waitpid(pid, &status, WUNTRACED);
            // tcsetpgrp(shell_pid, getpgrp());

            return;
        }
    }

    if (!found)
    {
        printf("No such process found\n");
    }
}


void bg_change(pid_t pid)
{
    int found = 0;

    for (int i = 0; i < bg_count_process; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            found = 1;

            if (kill(pid, 0) == -1)
            {
                if (errno == ESRCH)
                {
                    printf("No such process found\n");
                }
                else
                {
                    perror("kill");
                }
                return;
            }


            if (kill(pid, SIGCONT) == -1)
            {
                perror("kill");
            }
            else
            {
                printf("[%d] Process resumed in background\n", pid);
            }
        }
    }

    if (!found)
    {
        printf("No such process found\n");
    }
}

void add_bg_process(pid_t pid, char *name)
{
    bg_processes[bg_count_process].pid = pid;
    strncpy(bg_processes[bg_count_process].name, name, sizeof(bg_processes[bg_count_process].name) - 1);
    bg_processes[bg_count_process].is_stopped = 0;
    bg_count_process++;
}

void mark_process_stopped(pid_t pid)
{
    for (int i = 0; i < bg_count_process; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            bg_processes[i].is_stopped = 1;
            break;
        }
    }
}

void remove_from_bg_queue(pid_t pid)
{
    for (int i = 0; i < bg_count_process; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            for (int j = i; j < bg_count_process - 1; j++)
            {
                bg_processes[j] = bg_processes[j + 1];
            }
            bg_count_process--;
            return;
        }
    }
}