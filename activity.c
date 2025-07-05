#include "globals.h"
#include "input.h"
#include "activity.h"

#define MAX_PROCESSES 1024
#define MAX_COMMAND_LENGTH 1024
#define PROC_DIRECTORY "/proc"
#define STATUS_FILE "status"


Process process_queue[1024];
int process_count = 0;

void add_process_to_queue(pid_t pid, const char *command) 
{
    if (process_count < MAX_PROCESSES) 
    {
        process_queue[process_count].pid = pid;

        strncpy(process_queue[process_count].command, command, MAX_COMMAND_LENGTH - 1);
        process_queue[process_count].command[MAX_COMMAND_LENGTH - 1] = '\0';

        process_count++;
    } 
    else 
    {
        fprintf(stderr, "Maximum number of processes reached.\n");
    }
}

int is_numeric(const char *str) 
{
    for (int i = 0; str[i]; i++) 
    {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int read_process(const char *pid, char *state)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s/%s", PROC_DIRECTORY, pid, STATUS_FILE);

    FILE *file = fopen(path, "r");
    if (file == NULL) 
    {
        // perror("Failed to open stat/ file");
        return 0;
    }

    char line[512];
   while (fgets(line, sizeof(line), file)) 
    {
        if (strncmp(line, "State:", 6) == 0) 
        {
            sscanf(line, "State:\t%c", state);
        }
    }
    fclose(file);
    return 1;

}

const char* get_state_description(char state) 
{
    switch (state) {
        case 'R': return "Running";
        case 'S': return "Running";
        case 'T': return "Stopped";
        case 'Z': return "Zombie";
        default: return "Unknown";
    }
}

void activities()
{
    char state;
    pid_t pid;
    struct dirent *entry;
    // DIR *dir = opendir(PROC_DIRECTORY);


    char commands[1024];

    for (int i = 0; i < process_count; i++)
     {
        char pid_str[10];
        snprintf(pid_str, sizeof(pid_str), "%d", process_queue[i].pid);

        if (read_process(pid_str, &state)) 
        {
            if (state == 'K') {
                continue; 
            }
            printf("[%d] : %s - %s\n", process_queue[i].pid, process_queue[i].command, get_state_description(state));
        } 
    }

}