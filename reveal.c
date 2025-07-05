#include "display.h"
#include "input.h"
#include "reveal.h"
#include "log.h"
#include "globals.h"

#define RESET_COLOR "\033[0m"

int l = 0, a = 0;

int compare_entries(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_extended(struct stat *file, char *color, char *name)
{
    printf("%s", color);
    printf((S_ISDIR(file->st_mode)) ? "d" : "-");
    printf((file->st_mode & S_IRUSR) ? "r" : "-");
    printf((file->st_mode & S_IWUSR) ? "w" : "-");
    printf((file->st_mode & S_IXUSR) ? "x" : "-");
    printf((file->st_mode & S_IRGRP) ? "r" : "-");
    printf((file->st_mode & S_IWGRP) ? "w" : "-");
    printf((file->st_mode & S_IXGRP) ? "x" : "-");
    printf((file->st_mode & S_IROTH) ? "r" : "-");
    printf((file->st_mode & S_IWOTH) ? "w" : "-");
    printf((file->st_mode & S_IXOTH) ? "x" : "-");

    printf(" %2hu", file->st_nlink);

    struct passwd *pw = getpwuid(file->st_uid);
    printf(" %-8s", pw->pw_name);

    struct group *gr = getgrgid(file->st_gid);
    printf(" %-5s", gr->gr_name);

    printf(" %5lld", (long long)file->st_size);

    char timebuf[1000];
    struct tm *timeinfo = localtime(&file->st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", timeinfo);
    printf(" %s", timebuf);

    printf(" %s\n", name);

    printf(RESET_COLOR);
}

void print_details(char *name, char *path)
{
    if (a == 0)
    {
        if (name[0] == '.')
            return;
    }

    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, name);

    struct stat file;
    int s = stat(full_path, &file);

    if (s != 0)
    {
        perror("stat");
    }

    char *color = "\e[0;37m";
    if (S_ISDIR(file.st_mode))
        color = "\e[0;34m";
    else if (file.st_mode & S_IXUSR)
        color = "\033[1;32m";

    if (l != 0)
    {
        print_extended(&file, color, name);
    }
    else
    {
        printf("%s%s\n", color, name);
        printf(RESET_COLOR);
    }

    // add_to_log(store);
}

int get_details(char *dir)
{
    DIR *path = opendir(dir);
    if (path == NULL)
    {
        perror("opendir");
        return 0;
    }
    char *entries[5000];
    int count = 0;
    struct dirent *files;
    while ((files = readdir(path)) != NULL)
    {
        entries[count] = malloc(strlen(files->d_name) + 10);
        strcpy(entries[count], files->d_name);
        count++;
    }

    closedir(path);
    qsort(entries, count, sizeof(char *), compare_entries);

    if (l != 0)
        printf("total: %d\n", count);

    for (int i = 0; i < count; i++)
    {
        // printf("%s\n",entries[i]);
        print_details(entries[i], dir);
        free(entries[i]);
    }
    return 1;
}

void reveal(char *command)
{
    char flags[PATH_MAX];
    char path[PATH_MAX];

    flags[0] = '\0';
    path[0] = '\0';

    int fl = 0;
    int pl = 0;

    l = 0, a = 0;
    // char *args = strtok(command, " ");
    // args = strtok(NULL, " ");

    const char *delimiter = " ";
    char *saveptr;
    char *args = strtok_r(command, delimiter, &saveptr);
    args = strtok_r(NULL, delimiter, &saveptr);

    if (args == NULL)
    {
        get_dir(NULL, 1);
    }
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

            if (start[0] == '-' && pl == 0 && strlen(start) >= 2)
            {
                for (int i = 1; i < strlen(start); i++)
                {
                    if (start[i] == 'a' && a == 0)
                    {
                        flags[fl] = start[i];
                        fl++;
                        a = 1;
                    }
                    else if (start[i] == 'l' && l == 0)
                    {
                        flags[fl] = start[i];
                        fl++;
                        l = 1;
                    }
                }
            }
            else
            {
                strcpy(path, start);
                get_dir(path, 1);
                pl++;
            }

            // args = strtok(NULL, " ");
            args = strtok_r(NULL, delimiter, &saveptr);
        }
        if (path[0] == '\0')
        {

            get_dir(NULL, 1);
        }
    }
}

// int main()
// {
//     if (getcwd(shell_home_directory, sizeof(shell_home_directory)) == NULL)
//     {
//         perror("getcwd");
//         // exit(1);
//     }
//     char input[MAX_INPUT];

//     while (1)
//     {
//         display_username_and_system();
//         // printf("%s\n", shell_home_directory);
//         if (fgets(input, sizeof(input), stdin) == NULL)
//         {
//             printf("\n");
//             break;
//         }
//         long long len = strlen(input);
//         if (len > 0 && input[len - 1] == '\n')
//         {
//             input[len - 1] = '\0';
//         }
//         // check_cd(input);
//         check_commands(input);
//     }
// }