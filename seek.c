#include "display.h"
#include "input.h"
#include "seek.h"
#include "log.h"
#include "globals.h"

int d = 1, f = 1, e = 0;
#define RESET_COLOR "\033[0m"
int count = 0;
char *target, *source;

void seek_directory(const char *target, const char *dir)
{
    // printf("%s\n", dir);
    char *buf = malloc(4096);
    DIR *di = opendir(dir);
    if (di == NULL)
    {
        perror("opendir");
        return;
    }
    char *color = "\e[0;37m";
    struct stat statbuf;
    char *path = (char *)malloc(8192);
    char *rel_path = (char *)malloc(8192);
    struct dirent *files;
    int file = 0, directory = 0;
    char *go = (char *)malloc(8192);
    while ((files = readdir(di)) != NULL)
    {
        snprintf(path, strlen(dir) + 1 + strlen(files->d_name) + 1, "%s/%s", dir, files->d_name);

        if (stat(path, &statbuf) == -1)
        {
            perror("stat");
            continue;
        }

        char* s = malloc(sizeof(char)*4096);
        strcpy(s,files->d_name);

        char *dot = strrchr(s, '.');
        size_t len = (dot != NULL) ? (size_t)(dot - s) : strlen(s);

        if (strncmp(s, target, len) == 0 && target[len] == '\0')
        {
            if ((S_ISDIR(statbuf.st_mode) && d == 1) || (S_ISREG(statbuf.st_mode) && f == 1))
            {
                if (strcmp(dir, source) == 0)
                    snprintf(rel_path, strlen(path)+1, ".%s", path + strlen(dir));
                else
                    snprintf(rel_path, strlen(path)+1, ".%s", path+strlen(source));

                if (S_ISDIR(statbuf.st_mode))
                {
                    color = "\033[0;34m";
                    printf("%s%s\n", color, rel_path);
                }
                else
                {
                    color = "\033[1;32m";
                    printf("%s%s\n", color, rel_path);
                }
                printf(RESET_COLOR);
                count += 1;
                strcpy(go, path);
                if (S_ISDIR(statbuf.st_mode))
                    directory = 1;
                else if (S_ISREG(statbuf.st_mode))
                    file = 1;
            }
        }
        else if (strncmp(files->d_name, target, strlen(target)) == 0)
        {
            if ((S_ISDIR(statbuf.st_mode) && d == 1) || (S_ISREG(statbuf.st_mode) && f == 1))
            {
                if (strcmp(dir, source) == 0)
                    snprintf(rel_path, strlen(path)+1, ".%s", path + strlen(dir));
                else
                    snprintf(rel_path, strlen(path)+1, "%s", path);

                if (S_ISDIR(statbuf.st_mode))
                {
                    color = "\033[0;34m";
                    printf("%s%s\n", color, rel_path);
                }
                else
                {
                    color = "\033[1;32m";
                    printf("%s%s\n", color, rel_path);
                }
                printf(RESET_COLOR);
                count += 1;
                strcpy(go, path);
                if (S_ISDIR(statbuf.st_mode))
                    directory = 1;
                else if (S_ISREG(statbuf.st_mode))
                    file = 1;
            }
        }



        if (S_ISDIR(statbuf.st_mode) && strcmp(files->d_name, ".") != 0 && strcmp(files->d_name, "..") != 0)
        {
            seek_directory(target, path);
        }
    }
    closedir(di);

    if (e == 1)
    {
        if (count == 1)
        {
            if (file)
            {
                FILE *f = fopen(go, "r");
                if (f)
                {
                    char buffer[4096];
                    while (fgets(buffer, sizeof(buffer), f))
                    {
                        printf("%s", buffer);
                    }
                    fclose(f);
                    printf("\n");
                }
                else
                    printf("Missing permissions for task!\n");
            }
            else if (directory)
            {
                if (chdir(go) == -1)
                {
                    printf("Missing permissions for task!\n");
                }
            }
        }
    }

    // add_to_log(store);
}

void seek(char *command)
{

    char flags[PATH_MAX];
    target = malloc(4096);
    source = malloc(4096);

    flags[0] = '\0';
    source[0] = '\0';

    int fl = 0;
    int pl = 0;
    d = 1, f = 1, e = 0;

    const char *delimiter = " ";
    char *saveptr;
    char *args = strtok_r(command, delimiter, &saveptr);
    args = strtok_r(NULL, delimiter, &saveptr);

    while (args != NULL)
    {
        char *start = args;
        // printf("%s\n",start);
        while (isspace((unsigned char)*start))
            start += 1;
        char *end = start + strlen(start) - 1;
        while (isspace((unsigned char)*end) && end > start)
            end--;
        *(end + 1) = '\0';
        if (start[0] == '-' && strlen(start) >= 2 && pl == 0)
        {
            for (int i = 1; i < strlen(start); i++)
            {
                if (start[i] == 'd')
                {
                    flags[fl] = start[i];
                    fl++;
                    f = 0;
                }
                else if (start[i] == 'f')
                {
                    flags[fl] = start[i];
                    fl++;
                    d = 0;
                }
                else if (start[i] == 'e')
                {
                    flags[fl] = start[i];
                    fl++;
                    e = 1;
                }
            }
        }
        else if (pl == 0)
        {
            
            target = start;
            pl++;
        }
        else
        {
            // printf("wrefh3hrfh    %s",start);
            strcpy(source,start);
            source = start;
            pl++;
        }

        if (d == 0 && f == 0)
        {
            printf("Invalid Flags\n");
            return;
        }
        // printf("%s %s %s\n", flags, target, source);
        count = 0;

        args = strtok_r(NULL, delimiter, &saveptr);
    }
   if(strcmp(".",source) == 0|| strcmp("~",source) == 0 || strlen(source) == 0)
    {
        source = shell_home_directory;
        seek_directory(target,shell_home_directory);
    }
    else if(strcmp("-",source) == 0)
    {
        source = previous_dir;
        seek_directory(target,previous_dir);
    }
    else
    {
        // printf("qf2j %s\n", source);
        seek_directory(target, source);
    }

    if (count == 0)
        printf("No match found!\n");
}