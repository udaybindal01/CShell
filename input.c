#include "input.h"
#include "display.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "foreground.h"
#include "seek.h"
#include "globals.h"
#include "proclore.h"
#include "inout.h"
#include "activity.h"
#include "signal.h"
#include "iman.h"
#include "neonate.h"
#include "myshrc.h"

char store2[4096];

char *trim_whitespace(char *str)
{
    // printf("%s\n",str);
    char *end;
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = 0;

    return str;
}

void execute_pipes_redirection(char **commands, int num_commands, char *input_file, char *output_file, int append_mode)
{
    int pipe_fds[2];
    int input_fd = 0;
    int output_fd = 1;

    if (input_file != NULL)
    {
        input_fd = open(input_file, O_RDONLY);
        if (input_fd < 0)
        {
            perror("Failed to open input file");
            return;
        }
    }

    for (int i = 0; i < num_commands; i++)
    {
        if (i < num_commands - 1)
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (input_fd != 0)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            if (i == num_commands - 1 && output_file != NULL)
            {
                if (append_mode)
                {
                    output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                else
                {
                    output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                if (output_fd < 0)
                {
                    perror("Failed to open output file");
                    exit(EXIT_FAILURE);
                }
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            else if (i < num_commands - 1)
            {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }

            execute_command(trim_whitespace(commands[i]), 0, STDIN_FILENO, STDOUT_FILENO);
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
            if (input_fd != 0)
                close(input_fd);
            if (i < num_commands - 1)
            {
                close(pipe_fds[1]);
            }
            input_fd = pipe_fds[0];
        }
    }
}

void execute_command(char *command, int background, int input_fd, int output_fd)
{
    strcpy(store2, command);
    int f = 0;
    char *backg = strrchr(store2, '&');

    if (backg != NULL && *(backg + 1) == '\0')
    {
        f = 1;
        *backg = '\0';
    }

    char original_command[1024];
    strcpy(original_command, command);

    char *space = strchr(original_command, ' ');
    int command_length = space ? (int)(space - original_command) : strlen(original_command);

    for (int i = 0; i < alias_count; i++)
    {
        if (strncmp(original_command, aliases[i].name, command_length) == 0 && strlen(aliases[i].name) == command_length)
        {
            strcpy(command, aliases[i].command);
            if (space)
            {
                strcat(command, space);
            }
            break;
        }
    }

    fflush(stdout);

    if (saved_stdins < 0 || saved_stdouts < 0)
    {
        perror("Failed to save stdin/stdout");
        exit(EXIT_FAILURE);
    }

    if (input_fd != STDIN_FILENO)
    {
        // printf("1\n");
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    if (output_fd != STDOUT_FILENO)
    {
        // printf("2\n");
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    if (strncmp(command, "activities", 10) == 0)
    {
        activities();
    }
    else if (strncmp(command, "ping", 4) == 0)
    {
        ping(command);
    }
    else if (strncmp(command, "neonate", 7) == 0)
    {
        neonate(command);
    }
    else if (command[0] == 'h' && command[1] == 'o' && command[2] == 'p')
    {
        hop(command);
    }
    else if (command[0] == 'r' && command[1] == 'e' && command[2] == 'v' && command[3] == 'e' && command[4] == 'a' && command[5] == 'l')
        reveal(command);
    else if (command[0] == 'l' && command[1] == 'o' && command[2] == 'g')
    {
        flag = 1;
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
    else if (command[0] == 'i' && command[1] == 'M' && command[2] == 'a' && command[3] == 'n')
    {
        parseiMan(command);
    }
    else
    {
        // printf("%s\n",command);
        if (f == 1)
        {
            background = 1;
        }
        foreg(store2, background);
    }

    dup2(saved_stdins, STDIN_FILENO);
    dup2(saved_stdouts, STDOUT_FILENO);
    // printf("DONE\n");
    // close(saved_stdins);
    // close(saved_stdouts);
}

void check_commands(char *input)
{

    char *delimiter = ";";
    char *delimiter2 = "|";
    char *saveptr, *saveptr2, *saveptr3;
    char *point;
    char *new = malloc(sizeof(char) * 4096);
    strcpy(new, input);
    char *com = strtok_r(new, delimiter, &saveptr);
    char *new2 = malloc(sizeof(char) * 4096);
    char *pipe_commands[100];
    int num_commands = 0;
    int append = 0;

    char *input_file = NULL;
    char *output_file = NULL;
    char command_copy[MAX_INPUT];
    strncpy(command_copy, input, sizeof(command_copy));

    char *output_redir = strchr(command_copy, '>');

    if (output_redir != NULL)
    {
        if (*(output_redir + 1) == '>')
        {

            append = 1;
            *output_redir = '\0';
            output_redir++;
        }
        *output_redir = '\0';
        output_file = strtok(output_redir + 1, " ");
        if (output_file == NULL)
        {
            fprintf(stderr, "No output file specified!\n");
            return;
        }
        else
        {
            output_file = strtok(output_file, " \t\n");
        }
    }

    // printf("%d %s\n",append,output_file);

    char *input_redir = strchr(command_copy, '<');

    if (input_redir != NULL)
    {
        *input_redir = '\0';
        input_file = strtok(input_redir + 1, " ");
        if (input_file == NULL)
        {
            fprintf(stderr, "No input file specified!\n");
            return;
        }
        else
        {
            input_file = strtok(input_file, " \t\n");
        }
    }

    // printf("%s %s\n",input_file,output_file);

    while (com != NULL)
    {
        strcpy(new2, com);
        char *sub = strtok_r(com, "&", &saveptr2);

        while (sub != NULL)
        {
            char *buffer = malloc(sizeof(char) * strlen(sub));
            strcpy(buffer, sub);
            if (sub != NULL && strcmp(sub, new2) != 0)
            {
                strcat(buffer, "&");
            }
            if (saveptr2 != NULL)
                strcpy(new2, saveptr2);

            char *start = buffer;

            while (isspace((unsigned char)*start))
                start += 1;
            char *end = start + strlen(start) - 1;
            while (isspace((unsigned char)*end) && end > start)
                end--;
            *(end + 1) = '\0';

            if (strlen(start) == 0)
            {
                printf("Invalid use of pipe\n");
                return;
            }

            num_commands = 0;
            // printf("%s\n", start);
            if (start[strlen(start) - 1] == '|')
            {
                printf("Invalid use of pipes\n");
                return;
            }
            char *pipe_section = strtok_r(start, "|", &saveptr3);

            while (pipe_section != NULL)
            {
                pipe_commands[num_commands++] = pipe_section;

                pipe_section = strtok_r(NULL, "|", &saveptr3);
            }

            int flag = 0;
            strcpy(store, start);

            char *backg = strrchr(start, '&');

            if (backg != NULL && *(backg + 1) == '\0')
            {
                flag = 1;
                *backg = '\0';
            }

            for (int i = 0; i < num_commands; i++)
            {
                char *trimmed_command = trim_whitespace(pipe_commands[i]);
                if (strlen(trimmed_command) == 0)
                {
                    printf("Invalid use of pipe\n");
                    return;
                }
            }

            int len = strlen(start);
            if (num_commands == 1)
            {
                execute_command(trim_whitespace(pipe_commands[0]), flag, (input_file ? open(input_file, O_RDONLY) : STDIN_FILENO), (output_file ? open(output_file, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644) : STDOUT_FILENO));
                // handle_redirection(pipe_commands[0],flag);
            }
            else
            {
                execute_pipes_redirection(pipe_commands, num_commands, input_file, output_file, append);
            }

            sub = strtok_r(NULL, "&", &saveptr2);
        }
        com = strtok_r(NULL, delimiter, &saveptr);
    }
    // printf("HERE is the error\n");
    free(new);
    free(new2);
}

void get_dir(char *dir, int flag)
{
    char path[8192];
    char path_store[PATH_MAX];

    if (dir == NULL)
    {
        if (flag == 0)
        {
            strcpy(path, shell_home_directory);
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
        strcpy(path, shell_home_directory);
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
            strcpy(path, shell_home_directory);
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