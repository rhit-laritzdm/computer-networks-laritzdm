/*
** server.c -- a stream socket server
 * Used beej.us/guide/bgnet/html for help
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
#include <sys/wait.h>
#include <signal.h>

#define BACKLOG 10 // how many pending connections the queue will hold

bool readLine(char** line, size_t* size, size_t* length);

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int sockfd_s, sockfd_c; // listen on sockfd_s (server), new connection on sockfd_c (client)
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage client_addr; // client's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    int rv; // stands for return value
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: server port\n");
        exit(1);
    }

    // Allocates memory for "hints" and sets it to all zeros
    memset(&hints, 0, sizeof hints);
    // works for IPv4 or future types
    hints.ai_family = AF_UNSPEC;
    // SOCK_STREAM means TCP (SOCK_DGRAM means UDP)
    hints.ai_socktype = SOCK_STREAM;
    // use my IP address
    hints.ai_flags = AI_PASSIVE;

    // will fill in servinfo
    rv = getaddrinfo(NULL, argv[1], &hints, &servinfo);
    if(rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through servinfo (linked list) results and bind to first one that works
    for(p = servinfo; p != NULL; p = p->ai_next) {
        // create socket with info from servinfo (and error check)
        sockfd_s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd_s == -1) {
            perror("server: socket");
            continue;
        }

        // If able to create socket but can't set its options, then something else is wrong so close program
        if (setsockopt(sockfd_s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        // attempt to bind to the socket (and error check)
        if (bind(sockfd_s, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd_s);
            perror("server: bind");
        }

        // successfully bind to the socket, may exit loop now
        break;
    }

    freeaddrinfo(servinfo);

    // handle case where none of the items in servinfo linked list binded
    if (p == NULL) {
        fprintf(stderr, "server failed to bind\n");
        exit(1);
    }

    // start listening for a connection (up to BACKLOG=10 connections)
    if (listen(sockfd_s, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    // sigaction stuff helps handle forked processes
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) { // main accept loop
        sin_size = sizeof client_addr;
        sockfd_c = accept(sockfd_s, (struct sockaddr*)&client_addr, &sin_size);
        if (sockfd_c == -1) {
            perror("accept");
            continue;
        }

        // inet_ntop() means network to presentation (printable)
        // converts network address to human-readable string and saves to s
        inet_ntop(client_addr.ss_family,
                  get_in_addr((struct sockaddr *) &client_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // child process to handle a single connection
            close(sockfd_s); // child doesn't need listener
            // sends hello world message
            if (send(sockfd_c, "Hello, world!", 13, 0) == -1) {
                perror("send");
            }
            close(sockfd_c);
            exit(0);
        }
        close(sockfd_c); // parent doesn't need child processes' socket - make available for next connection
    }

    return 0;
}

// reads line entered in terminal and saves it to "char** line" variable
bool readLine(char** line, size_t* size, size_t* length) {
    while (1) {
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