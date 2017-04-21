// a server in the internet domain
// the pathname of the socket address is passed as an argument
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "server_functions.h"

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, sockfd_udp, length, n, pid;
     socklen_t clilen;
     socklen_t clilen2;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     socklen_t fromlen;
     struct sockaddr_in server;
     struct sockaddr_in from;
     char buf[1024];
     fd_set readfds;
     
     // outputs an error if no port is provided
     if (argc < 2){
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
   
     // creates a new TCP and UDP socket
     sockfd = makeSocket(SOCK_STREAM);
     sockfd_udp = makeSocket(SOCK_DGRAM);
   
     // sets the values in serv_addr
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
   
     // binds each socket to the port and outputs an error if it fails
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
         error("ERROR on binding");
     if (bind(sockfd_udp, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
         error("ERROR on binding");
     
     // listens to each socket and outputs an error if it fails
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     listen(sockfd_udp,5);
     clilen2 = sizeof(cli_addr);
        
     // clear the set
     FD_ZERO(&readfds);
   
     // add sockfd and sockfd_udp to the set
     FD_SET(sockfd, &readfds);
     FD_SET(sockfd_udp, &readfds);
   
     // waits for either sockfd or sockfd_udp to be read ready
     int status = select(sockfd_udp+1, &readfds, NULL, NULL, NULL);
   
     // outputs an error if select fails
     if(status == -1)
         error("ERROR on selecting");
     
     // sockfd or sockfd_udp is read ready
     else
     {
         // sockfd is read ready
         if(FD_ISSET(sockfd, &readfds))
         {
            // accepts the TCP socket and outputs an error if accept fails
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0)
                 error("ERROR on accept");
            
            // receives a message from the client and places it in buffer, outputs an error if it fails
            bzero(buffer,256);
            n = read(newsockfd,buffer,255);
            if (n < 0)
               error("ERROR reading from socket");
            
            // outputs the message received from the client to the standard output
            printf("Here is the message: %s",buffer);
            
            // sends a message to the client, outputs an error if it fails
            n = write(newsockfd,"I got your message",18);
            if (n < 0)
               error("ERROR writing to socket");
            
            // non-blocking wait for zombie processes
            //signal(SIGCHLD,SigCatcher);
            
            // continuous server that creates a child process
            while (1)
            {
               // accepts the TCP socket and outputs an error if it fails
               newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
               if (newsockfd < 0) 
                  error("ERROR on accept");
               
               // creates a child process and outputs an error if it fails
               pid = fork();
               if (pid < 0)
                  error("ERROR on fork");
               
               // the child process
               if (pid == 0) 
               {
                  // close the old socket
                  close(sockfd);
                  
                  // receive and send messages with the client
                  dostuff(newsockfd);
                  
                  // exit the child process
                  exit(0);
               }
               
               // the parent process
               else
                  // close the new socket
                  close(newsockfd);
            } /* end of while */
         }
        
         // sockfd_udp is read ready
         if(FD_ISSET(sockfd_udp, &readfds))
         {
            // sets the fromlen value
            fromlen = sizeof(struct sockaddr_in);
            
            // non-blocking wait for zombie processes
            //signal(SIGCHLD,SigCatcher);
            
            // continuous server that creates a child process
            while (1) {
               // creates a child process and outputs an error if it fails
               pid = fork();
               if (pid < 0)
                  error("ERROR on fork");
               
               // the child process
               if (pid == 0) 
               {
                  // receive message from the client and output an error if it fails
                  n = recvfrom(sockfd_udp, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
                  if (n < 0)
                     error("ERROR receiving from");
                  
                  // output the message received from the client to the standard output
                  write(1, "Received a datagram: ",21);
                  write(1,buf,n);
                  
                  // send a message to the client and output an error if it fails
                  n = sendto(sockfd_udp, "Got your message\n", 17, 0, (struct sockaddr *)&from, fromlen);
                  if (n < 0)
                     error("ERROR sending to");
                  
                  // exit the child process
                  exit(0);
               }
               
               // the parent process
               else
                  // close the socket
                  close(sockfd_udp);
            }
         } /* end of while */
        
     // close all sockets
     close(sockfd);
     close(sockfd_udp);
     close(newsockfd);
        
     return 0;
}
}
