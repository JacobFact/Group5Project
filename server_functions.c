// server_functions.c
// contains the common functions used in the server.c file

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <sys/wait.h>
#include "server_functions.h"

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
      
   // read from client
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0){
      error("ERROR reading from socket");
   }
   // display the message
   printf("Here is the message: %s\n",buffer);
   /// reply
   n = write(sock,"I got your message",18);
   if (n < 0){
       error("ERROR writing to socket");
   }
}

// used to deal with zombie processes
void *SigCatcher(int n)
{
  wait3(NULL,WNOHANG,NULL);
}

//prints an error message
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// creates a socket of the appropriate type
int makeSocket(int type)
{
    // creates a new socket
    int sockfd;
    sockfd = socket(AF_INET, type, 0);
        
    // outputs an error if the socket creation failed
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    // return the socket
    return sockfd;
}
