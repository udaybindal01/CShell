#include "input.h"
#include "display.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "globals.h"



int execute_hop(char *dir)
{
    char path[PATH_MAX];
    char path_store[PATH_MAX];


    if (getcwd(path_store, sizeof(path_store)) == NULL)
    {
        perror("getcwd");
    }

    strncpy(previous_dir, path_store, sizeof(previous_dir) - 1);
    previous_dir[sizeof(previous_dir) - 1] = '\0';
    // printf("%s\n",dir);

    if (chdir(dir) != 0)
    {
        perror("chdir");
        return 0;
        // exit(EXIT_FAILURE);
    }
    if (getcwd(path, sizeof(path)) != NULL)
    {
        printf("%s\n", path);
    }
    else
    {
        perror("getcwd");
        return 0;
    }

    // add_to_log(store);
    return 1;
}


void hop(char *command)
{

    const char *delimiter = " ";
    char *saveptr;
    char *args = strtok_r(command, delimiter, &saveptr);

    args = strtok_r(NULL, delimiter, &saveptr);
    if (args == NULL)
        get_dir(NULL,0);
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
            // printf("ewhrfh3r   %s\n",start);
            // printf("%s\n",start);
            get_dir(start,0);

            // args = strtok(NULL, " ");
            args = strtok_r(NULL, delimiter, &saveptr);
            // printf("args = %s\n",args);
        }
    }
}

