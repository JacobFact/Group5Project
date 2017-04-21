// client_functions.c
// contains the common functions of the client.c file

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

// prints the error message that is passed as an argument
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// creates a socket of the type specified
int makeSocket(int type)
{
    int sockfd;
    // creates a new socket
    sockfd = socket(AF_INET, type, 0);
        
    // outputs an error if the socket creation failed
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    // return the socket
    return sockfd;
}
