#include "globals.h"
#include "inout.h"
#include "input.h"
#include "pipe.h"

void handle_redirection(char *command, int flag)
{
    char *input_file = NULL;
    char *output_file = NULL;
    int append = 0;
    char command_copy[MAX_INPUT];
    strncpy(command_copy, command, sizeof(command_copy));
    
    // int saved_stdin = dup(STDIN_FILENO);
    // int saved_stdout = dup(STDOUT_FILENO);


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


    char *input_redir = strchr(command_copy, '<');
    // printf("%s %s\n",command,input_redir);
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

    pid_t p = fork();
    if (p == -1)
    {
        perror("fork failed");
        return;
    }
    else if (p == 0)
    {
        if (input_file != NULL)
        {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in == -1)
            {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        if (output_file != NULL) 
        {
            int fd_out;
            if (append) {
                fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (fd_out == -1) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        // printf("command = %s\n",command_copy);
        // check_commands(command_copy);
        execute_command(command,flag,STDIN_FILENO, STDOUT_FILENO);

    }
    else
    {
        wait(NULL);
    }
}
