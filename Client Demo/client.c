/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

bool readLine(char** line, size_t* size, size_t* length);

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 3) {
        fprintf(stderr,"usage: client hostname port\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    // works for IPv4 or future types
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    // reminder: & means address of
    // will fill in servinfo (rv for return value)
    if((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        // (always be checking for errors)
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    return 0;
}

bool readLine(char** line, size_t* size, size_t* length)
{
    while (1)
    {
        printf("string for the server> ");
        size_t len = getline(line, size, stdin);

        if(len == -1)
            return false;
        // dereferencing line
        // --len says decrease it first
        if((*line)[len-1] == '\n')
            (*line)[--len] = '\0';

        *length = len;

        if(len == 0)
            continue;

        // **line is first character of the array
        return len > 1 || **line != '.';
    }
}