#include "log.h"
#include "input.h"
#include "display.h"
#include "globals.h"

typedef struct log_entry
{
    char command[4096];
} LogEntry;

LogEntry log_entries[15];

int LOG_SIZE = 15;
int log_count = 0;
int curr = 0;

void load_log()
{
    FILE *file = fopen(logpath, "r");
    if (file == NULL)
    {
        // perror("fopen");
        return;
    }
    char buffer[4096];
    log_count = 0;

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (log_count < LOG_SIZE)
        {
            buffer[strcspn(buffer, "\n")] = '\0';
            strcpy(log_entries[log_count].command, buffer);
            log_count++;
        }
    }
    fclose(file);
}

void add_to_log(char *command)
{
    if (strncmp(command, "log", 3) == 0)
        return;

    if (log_count > 0)
    {
        if (strcmp(log_entries[log_count - 1].command, command) == 0)
            return;
    }

    if (log_count == LOG_SIZE)
    {
        for (int i = 1; i < LOG_SIZE; i++)
            strcpy(log_entries[i - 1].command, log_entries[i].command);

        log_count--;
    }

    strcpy(log_entries[log_count].command, command);

    log_count++;
    
    FILE *file = fopen(logpath, "w");
    if (file == NULL)
    {
        perror("fopen");
        return;
    }

    for (int i = 0; i < log_count; i++)
    {
        fprintf(file, "%s\n", log_entries[i].command);
    }

    fclose(file);
}

void log_execute(int n)
{
    if (n < 1 || n > log_count)
    {
        printf("Invalid index\n");
        return;
    }
    char* comm = malloc(sizeof(char)*4096);
    strcpy(comm,log_entries[log_count-n].command);
    // printf("%s %s\n",comm,log_entries[log_count-n].command);

    add_to_log(comm);
    check_commands(comm);
}

void log_purge()
{
    FILE *file = fopen(logpath, "w");
    if (file == NULL)
    {
        perror("fopen");
        return;
    }
    log_count = 0;
    fclose(file);
}

void show_log()
{
    for (int i = 0; i < log_count; i++)
    {
        printf("%d %s\n", log_count-i, log_entries[i].command);
    }
}

void logs(char *command)
{

    const char *delimiter = " ";
    char *saveptr;
    char *args = strtok_r(command, delimiter, &saveptr);
    args = strtok_r(NULL, delimiter, &saveptr);

    if (args == NULL)
    {
        show_log();
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

            if (strcmp(start, "execute") == 0)
            {
                args = strtok_r(NULL, delimiter, &saveptr);
                char *start_1 = args;
                while (isspace((unsigned char)*start_1))
                    start_1 += 1;
                char *end_1 = start_1 + strlen(start_1) - 1;
                while (isspace((unsigned char)*end_1) && end_1 > start_1)
                    end_1--;
                *(end_1 + 1) = '\0';

                log_execute(atoi(start_1));
            }
            else if (strcmp(start, "purge") == 0)
            {
                log_purge();
            }

            args = strtok_r(NULL, delimiter, &saveptr);
        }
    }
}
