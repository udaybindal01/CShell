#include "globals.h"
#include "myshrc.h"
#include "seek.h"
#include "hop.h"

struct Alias aliases[MAX_ALIAS];
struct Function functions[MAX_FUNCTION];
int alias_count = 0;
int function_count = 0;

void trim(char *str)
{
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '"' || str[len - 1] == '\''))
    {
        str[--len] = '\0';
    }
    while (*str && (*str == ' ' || *str == '\t' || *str == '"' || *str == '\''))
    {
        ++str;
        --len;
    }
    memmove(str, str, len + 1);
}

void load_myshrc()
{
    FILE *file = fopen(".myshrc", "r");
    if (file == NULL)
    {
        printf("Error opening .myshrc file\n");
        return;
    }

    char line[MAX_COMMAND];
    char function_body[MAX_FUNCTION_BODY] = "";
    int in_function = 0;
    char current_function[50] = "";

    while (fgets(line, sizeof(line), file))
    {

        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "alias ", 6) == 0)
        {
            char *name = strtok(line + 6, "=");
            char *command = strtok(NULL, "");
            if (name && command)
            {
                trim(name);
                trim(command);
                strcpy(aliases[alias_count].name, name);
                strcpy(aliases[alias_count].command, command + 1);
                // printf("%s %s\n", aliases[alias_count].name, aliases[alias_count].command);
                alias_count++;
            }
        }
        else if (strstr(line, "()") != NULL)
        {
            // printf("here1\n");
            in_function = 1;
            sscanf(line, "%[^()]", current_function);
            function_body[0] = '\0'; 
        }
        else if (in_function)
        {
            if (strcmp(line, "}") == 0)
            {

                in_function = 0;
                strcpy(functions[function_count].name, current_function);
                strcpy(functions[function_count].body, function_body);
                // printf("%s %s\n",functions[function_count].name, functions[function_count].body);
                function_count++;
            }
            else
            {

                strcat(function_body, line);
                strcat(function_body, "\n");
                
            }
        }
        else if (strchr(line, '=') != NULL && strncmp(line, "alias ", 6) != 0)
        {

            char *name = strtok(line, "=");
            char *command = strtok(NULL, "");
            if (name && command)
            {
                strcpy(aliases[alias_count].name, name);
                strcpy(aliases[alias_count].command, command + 1);
                alias_count++;
            }
        }
    }

    fclose(file);
}

void execute_function(const char *function_name)
{
    for (int i = 0; i < function_count; i++)
    {
        if (strcmp(function_name, functions[i].name) == 0)
        {

            printf("Executing function %s:\n%s\n", function_name, functions[i].body);
            return;
        }
    }
    printf("Function %s not found\n", function_name);
}

// void execute_command(char *command)
// {

//     for (int i = 0; i < alias_count; i++)
//     {
//         if (strcmp(command, aliases[i].name) == 0)
//         {
//             system(aliases[i].command);
//             return;
//         }
//     }

//     for (int i = 0; i < function_count; i++)
//     {
//         if (strcmp(command, functions[i].name) == 0)
//         {
//             printf("hldfhwl\n");
//             execute_function(functions[i].name);
//             return;
//         }
//     }

//     system(command);
// }

// int main()
// {
//     load_myshrc();

//     char command[MAX_COMMAND];
//     while (1)
//     {
//         printf("<JohnDoe@SYS:~> ");
//         fgets(command, sizeof(command), stdin);
//         command[strcspn(command, "\n")] = 0;

//         if (strcmp(command, "exit") == 0)
//         {
//             break;
//         }

//         execute_command(command);
//     }

//     return 0;
// }