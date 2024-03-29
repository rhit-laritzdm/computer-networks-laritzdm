/*
** client.c -- a stream socket client
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

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 1024 // max number of bytes we can get at once

bool readLine(char** line, size_t* size, size_t* length);

int connectToServer(int sockfd, struct addrinfo* servinfo, struct addrinfo* p);

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int sockfd, bytessend, bytesrecv; //totalbytessend, totalbytesrecv;
    char buf[MAXDATASIZE];
    char* line;
    size_t size;
    size_t length;
    struct addrinfo hints, *servinfo, *p;
    int rv; // stands for return value
    char s[INET6_ADDRSTRLEN];

    if (argc != 3) {
        fprintf(stderr,"usage: client hostname port\n");
        exit(1);
    }

    // Allocates memory for "hints" and sets it to all zeros
    memset(&hints, 0, sizeof hints);
    // works for IPv4 or future types
    hints.ai_family = AF_UNSPEC;
    // SOCK_STREAM means TCP (SOCK_DGRAM means UDP)
    hints.ai_socktype = SOCK_STREAM;

    // fills in servinfo with address info for server
    rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo);
    if(rv != 0) {
        // (always be checking for errors)
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through servinfo (linked list) results and connect to first one that works
    for(p = servinfo; p != NULL; p = p->ai_next) {
        // create socket with info from servinfo (and error check)
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            perror("client: socket");
            continue;
        }

        // attempt to connect to the socket (and error check)
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        // successfully connected to socket, may exit loop now
        break;
    }

    // handle case where none of the items in servinfo linked list connected
    if (p == NULL) {
        fprintf(stderr, "client failed to connect\n");
        return 2;
    }

    // inet_ntop() means network to presentation (printable)
    // converts network address to human-readable string and saves to s
    inet_ntop(p->ai_family,
              get_in_addr((struct sockaddr*) p->ai_addr),
                      s,
                      sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);

    // send messages to server
    // int send(int sockfd, const void *msg, int len, int flags);
    // ;;; is a protocol Wilkin made
    line = "temp string";
    while(strcmp(line, ";;;")) {
        // send a message
        //totalbytessend = 0;
        readLine(&line, &size, &length); // User types a message
        bytessend = send(sockfd, line, length, 0);
        if (bytessend == -1) {
            perror("send");
            exit(1);
        }
        printf("Sent message '%s'\n", line);
        /*
        while (totalbytessend < length) {
            bytessend = send(sockfd, line, length, 0);
            if (bytessend == -1) {
                perror("send");
                exit(1);
            }
            totalbytessent += bytessent;
        }
         */

        // recv a message
        bytesrecv = recv(sockfd, buf, MAXDATASIZE-1, 0);
        if (bytesrecv == -1) {
            perror("recv");
            exit(1); // I think this is the same as return(1);
        }
        buf[bytesrecv] = '\0';
        printf("Received from server: '%s'\n", buf);
    }


    /*
    // listens for message from server
    bytesrecv = recv(sockfd, buf, MAXDATASIZE-1, 0);
    if (bytesrecv == -1) {
        perror("recv");
        exit(1); // I think this is the same as return(1);
    }

    buf[bytesrecv] = '\0';

    // prints message from server to terminal
    printf("client: received '%s'\n", buf);
     */
    printf("closing socket\n");
    close(sockfd);
    return 0;
}

// reads line entered in terminal and saves it to "char** line" variable
bool readLine(char** line, size_t* size, size_t* length) {
    while (1) {
        printf("-> ");
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

        // return whether the sentinel value was sent
        return strcmp(*line, ";;;");
    }
}