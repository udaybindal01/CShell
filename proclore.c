#include "display.h"
#include "input.h"
#include "globals.h"

void exe_pro(pid_t pid)
{
    if (pid == -1)
        pid = getpid();

    char path[4096], buffer[4096];

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Failed to open stat file");
        return;
    }
    char status;
    pid_t pgrp, tpgid;
    long int vm;
    char p[10];

    char *info[25];

    fgets(buffer, 4096, file);
    // printf("%s\n", buffer);

    char *token = strtok(buffer, " ");
    info[0] = (char *)malloc(strlen(token));
    strcpy(info[0], token);
    token = strtok(NULL, " ");
    int in = 1;
    for (int i = 1; i < 25; i++)
    {
        // printf("HERE\n");
        info[i] = (char *)malloc(strlen(token) + 1);
        strcpy(info[in], token);
        // printf("%s\n",info[in]);
        in++;
        token = strtok(NULL, " ");
    }

    fclose(file);
    strcpy(p, info[2]);
    vm = atoi(info[22]);

    char path2[4096];
    snprintf(path2, sizeof(path2), "/proc/%d/exe", pid);
    int terminal_pgid = atoi(info[7]);
    int pgid = atoi(info[4]);

    char a[4096];
    memset(a, 0, sizeof(a));
    int len = readlink(path2, a, sizeof(a) - 1);
    if (len != -1)
    {
        a[len] = '\0';
    }
    char current[4096];
    strcpy(current, shell_home_directory);


    if (strncmp(a, current, strlen(current)) == 0)
    {
        // printf("HERE\n");
        memmove(a + 1, a + strlen(current), strlen(a) - strlen(current) + 1);
        a[0] = '~';
    }
    printf("pid : %d\n", pid);
    printf("process status : %s%c\n", p, (pgid == terminal_pgid) ? '+' : ' ');
    printf("Process Group : %d\n", pgid);
    printf("Virtual memory : %lu\n", vm);
    printf("executable path : %s\n", a);

}

void pro(char *command)
{
    const char *delimiter = " ";
    char *saveptr;
    char *args = strtok_r(command, delimiter, &saveptr);

    args = strtok_r(NULL, delimiter, &saveptr);
    if (args == NULL)
        exe_pro(-1);
    else
    {
        while (args != NULL)
        {
            char *start = args;

            while (isspace((unsigned char)*start))
                start += 1;
            char *end = start + strlen(start) - 1;
            while (isspace((unsigned char)*end) && end > start)
                end--;
            *(end + 1) = '\0';

            exe_pro(atoi(start));

            // args = strtok(NULL, " ");
            args = strtok_r(NULL, delimiter, &saveptr);
        }
    }
}