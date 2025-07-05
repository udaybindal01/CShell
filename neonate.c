#include "globals.h"
#include "neonate.h"
#include <errno.h>

volatile sig_atomic_t stop = 0;
struct termios orig_term;

#define MAX_PID 32768
#define PROC_PATH "/proc"

void get_latest_pid(int *latest_pid)
{
    *latest_pid = 0;
    time_t latest_time = 0;
    char path[256];
    struct stat statbuf;

    int fd = open("/proc/sys/kernel/ns_last_pid", O_RDONLY);
    ssize_t bytesRead;
    char buffer[100];
    bytesRead = read(fd, buffer, sizeof(buffer));
    close(fd);
    *latest_pid = atoi(buffer);

    if (*latest_pid == 0)
    {
        fprintf(stderr, "Warning: No valid PIDs found\n");
    }
}

void set_terminal_mode()
{
    struct termios new_term;
    if (tcgetattr(STDIN_FILENO, &orig_term) == -1)
        perror("tcgetattr");
    atexit(reset_terminal_mode);
    new_term = orig_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_term) == -1)
        perror("tcsetattr");
}

void reset_terminal_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}

// void get_latest_pid(int *latest_pid)
// {
//     DIR *proc_dir = opendir("/proc");
//     if (proc_dir == NULL)
//     {
//         perror("opendir");
//         exit(EXIT_FAILURE);
//     }

//     struct dirent *entry;
//     *latest_pid = 0;
//     int latest_time = 0;

//     while ((entry = readdir(proc_dir)) != NULL)
//     {
//         if (entry->d_type == DT_DIR)
//         {
//             char *endptr;
//             int pid = strtol(entry->d_name, &endptr, 10);
//             if (*endptr != '\0')
//                 continue;

//             char path[256];
//             snprintf(path, sizeof(path), "/proc/%d/stat", pid);

//             struct stat statbuf;
//             if (stat(path, &statbuf) == 0)
//             {
//                 if (statbuf.st_mtime > latest_time)
//                 {
//                     *latest_pid = pid;
//                     latest_time = statbuf.st_mtime;
//                 }
//             }
//         }
//     }
//     closedir(proc_dir);
// }

void neonate(char *command)
{
    char *saveptr, *saveptr2, *saveptr3;
    char *point;
    char *new = malloc(sizeof(char) * 4096);
    // strcpy(new, INPUT_MAIN);
    char *com = strtok_r(command, " ", &saveptr);
    com = strtok_r(NULL, " ", &saveptr);
    com = strtok_r(NULL, " ", &saveptr);
    char *str = com;
    while (*str)
    {
        if (*str == '.')
        {
            printf("Invalid time\n");
            return;
        }
        str++;
    }

    printf("time = %s\n", com);
    int t = atoi(com);
    if (t < 0)
    {
        printf("Invalid time\n");
        return;
    }
    else
    {
        pid_t p = fork();
        set_terminal_mode();
        if (p < 0)
        {
            perror("fork");
            return;
        }
        else if (p == 0)
        {
            while (1)
            {
                int latest_pid;
                get_latest_pid(&latest_pid);
                if (latest_pid)
                {
                    printf("%d\n", latest_pid);
                    fflush(stdout);
                }
                sleep(t);
            }
        }
        else
        {
            char c;
            while (1)
            {
                if (read(STDIN_FILENO, &c, 1) == 1)
                {
                    if (c == 'x' || c == 'X')
                    {
                        kill(p, SIGKILL);
                        break;
                    }
                }
            }
            wait(NULL);
        }
        reset_terminal_mode();
    }
}

void neo2()
{

    // printf("HERE\n");
    // neonate(INPUT);

    int interval = atoi(INPUT_MAIN);
    if (interval <= 0)
    {
        fprintf(stderr, "Invalid time_arg. Must be a positive integer.\n");
        // return EXIT_FAILURE;
    }
    struct termios orig_term2;
    set_terminal_mode(STDIN_FILENO, &orig_term2);

    fd_set read_fds;
    struct timeval timeout;
    int max_fd = STDIN_FILENO;

    while (!stop)
    {
        int latest_pid;
        get_latest_pid(&latest_pid);
        if (latest_pid != 0)
        {
            printf("%d\n", latest_pid);
            fflush(stdout);
        }

        // Set up the timeout for select
        timeout.tv_sec = interval;
        timeout.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        if (activity < 0)
        {
            perror("select");
            // return EXIT_FAILURE;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            int ch = getchar();
            if (ch == 'x' || ch == 'X')
            {
                stop = 1;
            }
        }
    }

    reset_terminal_mode(STDIN_FILENO, &orig_term);
    // return EXIT_SUCCESS;
}
