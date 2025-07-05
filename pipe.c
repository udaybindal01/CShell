// #include "globals.h"
// #include "input.h"
// #include "pipe.h"

// void execute_pipeline(char * command)
// {
//      char *commands[4096];
//     int count = 0;


//     char *token = strtok(command, "|");
//     while (token != NULL)
//     {
//         while (*token == ' ')
//             token++;
//         if (*token == '\0')
//         {
//             fprintf(stderr, "Invalid use of pipe\n");
//             exit(EXIT_FAILURE);
//         }
//         commands[count++] = token;
//         token = strtok(NULL, "|");
//     }

//     if (count == 0)
//     {
//         fprintf(stderr, "Invalid use of pipe\n");
//         exit(EXIT_FAILURE);
//     }

//     commands[count] = NULL;

//     int pipe_fds[2 * (count - 1)];
//     for (int i = 0; i < count - 1; ++i)
//     {
//         if (pipe(pipe_fds + 2 * i) < 0)
//         {
//             perror("pipe");
//             exit(EXIT_FAILURE);
//         }
//     }

//     for (int i = 0; i < count; ++i)
//     {
//         pid_t pid = fork();
//         if (pid < 0)
//         {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }

//         if (pid == 0)
//         {
            
//             if (i < count - 1)
//             {
//                 printf("%d\n",pipe_fds[2 * i + 1]);
//                 if (dup2(pipe_fds[2 * i + 1], STDOUT_FILENO) < 0)
//                 {
//                     perror("dup2 here is the error");
//                     // exit(EXIT_FAILURE);
//                 }
//             }

//             if (i > 0)
//             {
//                 printf("%d\n",pipe_fds[2 * (i-1)]);
//                 if (dup2(pipe_fds[2 * (i - 1)], STDIN_FILENO) < 0)
//                 {
//                     perror("dup2 3fhh13rfho1r");
//                     // exit(EXIT_FAILURE);
//                 }
//             }
            

//             for (int j = 0; j < 2 * (count - 1); ++j)
//             {
//                 close(pipe_fds[j]);
//             }

//             // Execute the command
//             // char *args[1000];
//             // char *token = strtok(commands[i], " ");
//             // int arg_count = 0;
//             // while (token != NULL)
//             // {
//             //     args[arg_count++] = token;
//             //     token = strtok(NULL, " ");
//             // }
//             // args[arg_count] = NULL;

//             // execvp(args[0], args);
//             // perror("execvp");
//             // exit(EXIT_FAILURE);
//             execute_command(commands[i],0);
//         }
//     }


//     for (int i = 0; i < 2 * (count - 1); ++i)
//     {
//         close(pipe_fds[i]);
//     }

//     for (int i = 0; i < count; ++i)
//     {
//         wait(NULL);
//     }
// }


