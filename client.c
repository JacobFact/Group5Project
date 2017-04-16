// client.c
// A client for udp and tcp in the internet domain

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include "client_functions.h"

int main(int argc, char *argv[])
{
    // determine whether UDP or TCP is being used
    printf("UDP or TCP: ");
    fgets(answerstr,3,stdin);
    
    // if TCP is being used
    if(answerstr=="TCP")
    {
        int sockfd, portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[256];
        
        // outputs an error if no hostname port is provided
        if (argc < 3) {
           fprintf(stderr,"usage %s hostname port\n", argv[0]);
           exit(0);
        }
        
        // sets the portno to be the user provided value
        portno = atoi(argv[2]);
        
        // creates a new TCP socket
        sockfd = makeSocket(SOCK_STREAM);
        
        // locates the host and outputs an error if the host was not found
        server = gethostbyname(argv[1]);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            exit(0);
        }
        
        // sets the values in serv_addr
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(portno);
        
        // connects with host, outputs an error if connect fails
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR connecting");
        
        // receives a message from the user and places it in buffer
        printf("Please enter the message: ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        
        // sends the message from the user to the server, outputs an error if it fails
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        
        // receives a message from the server, outputs an error if it fails
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0) 
             error("ERROR reading from socket");
        
        // outputs the message received from the server
        printf("%s\n",buffer);
        
        // closes the socket
        close(sockfd);
        
        return 0;
    }
    
    // if UDP is being used
    else if(answerstr=="UDP")
    {
       int sock, n;
       unsigned int length;
       struct sockaddr_in server, from;
       struct hostent *hp;
       char buffer[256];

       // outputs an error if no server port is provided
       if (argc != 3) {
           printf("Usage: server port\n");
           exit(1);
       }
        
       // creates a new UDP socket
       sock= makeSocket(SOCK_DGRAM);

       // sets value of sin_family in server
       server.sin_family = AF_INET;
        
       // locates the host and outputs an error if the host was not found
       hp = gethostbyname(argv[1]);
       if (hp==0) error("Unknown host");

       // sets the values in server
       bcopy((char *)hp->h_addr, 
            (char *)&server.sin_addr,
             hp->h_length);
       server.sin_port = htons(atoi(argv[2]));
       length=sizeof(struct sockaddr_in);
        
       // receives a message from the user and places it in buffer
       printf("Please enter the message: ");
       bzero(buffer,256);
       fgets(buffer,255,stdin);
        
       // sends the message from the user to the server, outputs an error if it fails
       n=sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);
       if (n < 0) 
           error("Sendto");
        
       // receives a message from the server, outputs an error if it fails
       n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
       if (n < 0)
           error("recvfrom");
       
       // writes the message to the standard output
       write(1,"Got an ack: ",12);
       write(1,buffer,n);
        
       // closes the socket
       close(sock);
       
       return 0;
    }
    
    // if input is not UDP or TCP
    else
        error("ERROR: Must answer UDP or TCP");
}

// creates a socket of the type specified
int makeSocket(int type)
{
    // creates a new socket
    sockfd = socket(AF_INET, type, 0);
        
    // outputs an error if the socket creation failed
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    // return the socket
    return sockfd;
}
