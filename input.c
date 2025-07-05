#include "input.h"
#include "display.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "foreground.h"
#include "seek.h"
#include "globals.h"
#include "proclore.h"

void execute_command(char *command, int background)
{
    // printf("%s\n",command);

    if (command[0] == 'h' && command[1] == 'o' && command[2] == 'p')
    {
        hop(command);
    }
    else if (command[0] == 'r' && command[1] == 'e' && command[2] == 'v' && command[3] == 'e' && command[4] == 'a' && command[5] == 'l')
        reveal(command);
    else if (command[0] == 'l' && command[1] == 'o' && command[2] == 'g')
    {
        flag =1;
        logs(command);
    }
    else if (command[0] == 's' && command[1] == 'e' && command[2] == 'e' && command[3] == 'k')
    {
        seek(command);
    }
    else if (command[0] == 'p' && command[1] == 'r' && command[2] == 'o' && command[3] == 'c' && command[4] == 'l' && command[5] == 'o' && command[6] == 'r' && command[7] == 'e')
    {
        pro(command);
    }
    else
    {
        foreg(command, background);
    }
}

void check_commands(char *input)
{

    char *delimiter = ";";
    char *saveptr, *saveptr2;
    char *point;
    char *new = malloc(sizeof(char) * 4096);
    strcpy(new, input);
    char *com = strtok_r(new, delimiter, &saveptr);
    char *new2 = malloc(sizeof(char) * 4096);
    while (com != NULL)
    {
        strcpy(new2, com);
        char *sub = strtok_r(com, "&", &saveptr2);

        while (sub != NULL)
        {
            // char *start = sub;

            char *buffer = malloc(sizeof(char) * strlen(sub));
            strcpy(buffer, sub);
            // printf("%s %s %s\n",sub,new2,saveptr2);
            if (sub != NULL && strcmp(sub, new2) != 0)
            {
                strcat(buffer, "&");
            }
            if(saveptr2!=NULL)
                strcpy(new2, saveptr2);

            char *start = buffer;

            while (isspace((unsigned char)*start))
                start += 1;
            char *end = start + strlen(start) - 1;
            while (isspace((unsigned char)*end) && end > start)
                end--;
            *(end + 1) = '\0';

            int flag = 0;
            strcpy(store, start);

            char *backg = strrchr(start, '&');

            if (backg != NULL && *(backg + 1) == '\0')
            {
                flag = 1;
                *backg = '\0';
            }

            int len = strlen(start);
            if (len > 0)
            {
                execute_command(start, flag);
            }
            sub = strtok_r(NULL, "&", &saveptr2);
        }
        com = strtok_r(NULL, delimiter, &saveptr);
    }
}

void get_dir(char *dir, int flag)
{
    char path[8192];
    char path_store[PATH_MAX];

    if (dir == NULL)
    {
        if (flag == 0)
        {
            strcpy(path,shell_home_directory);
            // dir = shell_home_directory;
        }
        else if (flag == 1)
        {
            if (getcwd(path, sizeof(path)) == NULL)
            {
                perror("getcwd");
                return;
            }
            // dir = path;
        }
    }
    else if (strcmp(dir, "~") == 0)
    {
        strcpy(path,shell_home_directory);
        // dir = shell_home_directory;
    }
    else if (strcmp(dir, ".") == 0)
    {
        if (getcwd(path, sizeof(path)) == NULL)
        {
            perror("getcwd");
            return;
        }
        // dir = path;
    }
    else if (strcmp(dir, "..") == 0)
    {
        if (getcwd(path, sizeof(path)) == NULL)
        {
            perror("getcwd");
            return;
        }
        strcat(path, "/..");
        // dir = path;
    }
    else if (strcmp(dir, "-") == 0)
    {

        if (strlen(previous_dir) == 0)
        {
            strcpy(path,shell_home_directory);
            // dir = shell_home_directory;
        }
        else
            strcpy(path, previous_dir);
    }
    else if (strncmp(dir, "~/", 2) == 0)
    {
        snprintf(path, sizeof(path), "%s/%s", shell_home_directory, dir + 2);
        // dir = path;
    }
    else
    {
        // printf("%s\n",current_director/y);
        snprintf(path, sizeof(path), "./%s", dir);
    }

    if (flag == 0)
    {
        // printf("%s\n",path);
        execute_hop(path);
    }
    else if (flag == 1)
    {
        get_details(path);
    }

    return;
}