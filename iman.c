#include "globals.h"
#include "input.h"
#include "iman.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUFFER 100000
#define HEADER_BUFFER_SIZE 8192

typedef struct
{
    const char *entity;
    char character;
} EntityPair;

static const EntityPair entityTable[] =
    {
        {"&lt;", '<'},
        {"&gt;", '>'},
        {"&amp;", '&'},
        {"&quot;", '"'},
        {"&apos;", '\''},
        {NULL, 0}};

char *cut(const char *input)
{
    if (input == NULL)
        return NULL;

    int inputLen = strlen(input);
    char *output = malloc(inputLen + 1);
    if (output == NULL)
        return NULL;

    int i = 0, j = 0;

    while (input[i])
    {
        if (input[i] == '&')
        {
            int found = 0;
            const EntityPair *pair = entityTable;

            while (pair->entity != NULL)
            {
                int entityLen = strlen(pair->entity);

                if (strncmp(&input[i], pair->entity, entityLen) == 0)
                {
                    output[j++] = pair->character;
                    i += entityLen;
                    found = 1;
                    break;
                    
                }
                pair++;
            }
            if (!found)
            {
                output[j++] = input[i++];
            }
        }
        else
        {
            output[j++] = input[i++];
        }
    }
    output[j] = '\0';

    return output;
}

void decode(char *input, int l)
{
    input = cut(input);

    char t[l + 1];
    int y = 0, flag = 0;

    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '#')
            flag = 1;

        if (input[i] == '<' && !flag)
            continue;
        if (input[i] == '>')
        {
            flag = 0;
            continue;
        }

        t[y++] = input[i];
    }

    t[y] = '\0';
    printf("%s\n", t);
}

void iMans(const char *command)
{
    struct addrinfo hints, *res;
    int sockfd;
    char buffer[BUFFER];
    buffer[0] = '\0';

    // char buffer_copy[BUFFER_SIZE];
    char header_buffer[HEADER_BUFFER_SIZE] = {0};
    int bytes_received;
    int header_end = 0;
    char *body_start;
    int total_received = 0;
    struct addrinfo *a;
    struct addrinfo *b;

    int filedescripter = -1;

    char request[1024];
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.0\r\n"
             "Host: man.he.net\r\n"
             "Connection: close\r\n\r\n",
             command);

    // printf("Prepared GET request:\n%s\n", request); // Debugging statement

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("man.he.net", "http", &hints, &res) != 0)
    {
        perror("getaddrinfo");
        exit(1);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1)
    {
        perror("socket");
        exit(1);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("connect");
        close(sockfd);
        exit(1);
    }
    printf("Connected to man.he.net...\n");

    freeaddrinfo(res);

    if (send(sockfd, request, strlen(request), 0) == -1)
    {
        perror("send");
        close(sockfd);
        exit(1);
    }
    // printf("GET request sent successfully...\n");

    char *html_start = NULL;
    char *strong_start = NULL;
    char *strong_end = NULL;
    char *pre_start = NULL;
    char *pre_end = NULL;
    char *start;
    char *end;
    char result[100000];
    result[0] = '\0';
    int first = 0;

    char *before_html_end = NULL;
    ssize_t n;
    while ((n = recv(sockfd, result, sizeof(result), 0)) > 0)
    {
        if (strstr(result, "No matches for") != NULL)
        {
            printf("Error: No such command\n");
            return;
        }

        char *start = strstr(result, "NAME\n");
        char *end = strstr(result, "</PRE>");
        size_t l;

        if (start == NULL && end == NULL && first != 0)
        {
            decode(result, strlen(result));
        }
        else if (end != NULL)
        {
            l = (start == NULL) ? (end - result) : (end - start);
            char *data = (char *)malloc(l + 1);

            if (data == NULL)
            {
                perror("Memory allocation failed");
                return;
            }

            strncpy(data, (start == NULL) ? result : start, l);
            data[l] = '\0';
            decode(data, l);

            free(data);
            // free(weburl);
            close(filedescripter);
            return;
        }
        else if (start != NULL)
        {
            first = 1;
            l = strlen(result) - (start - result);
            char *data = (char *)malloc(l + 1);

            if (data == NULL)
            {
                perror("Memory allocation failed");
                return;
            }

            strncpy(data, start, l);
            data[l] = '\0';
            decode(data, l);

            free(data);
        }
        else
        {
            // free(weburl);
            close(filedescripter);
            return;
        }
    }

    if (n == -1)
    {
        perror("recv");
    }

    close(sockfd);
}

void parseiMan(char *command)
{
    char *saveptr, *saveptr2, *saveptr3;
    char *point;
    char *new = malloc(sizeof(char) * 4096);
    strcpy(new, command);
    char *com = strtok_r(new, " ", &saveptr);
    com = strtok_r(NULL, " ", &saveptr);
    // printf("%s\n",com);
    iMans(com);
}