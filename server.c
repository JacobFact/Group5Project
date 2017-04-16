/* a server in the internet domain.  The pathname of 
   the socket address is passed as an argument */
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
     int sockfd, newsockfd, portno,sock;
     int length;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     socklen_t fromlen;
     struct sockaddr_in server;
     struct sockaddr_in from;
     char buf[1024];
     int n;
     /*if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     sockfdudp = socket(AF_INET, SOCK_DGRAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     if (sockfdudp < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     if (bind(sockfdudp, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     listen(sockfdudp,5);
     clilen2 = sizeof(cli_addr);*/
   
     //**//
     fd_set readfds;
     // clear the set
     FD_ZERO(&readfds);
     // add sockfd and sockfdupd to the set
     FD_SET(sockfd, &readfds);
     FD_SET(sockfdudp, &readfds);
     // waits for either sockfd or sockfdudp to be read ready
     int status = select(sockfdudp+1, &readfds, NULL, NULL, NULL);
     // check for error
     if(status == -1)
         error("ERROR on selecting");
     else
     {
         // sockfd is read ready
         if(FD_ISSET(sockfd, &readfds))
         {
           // do tcp portion
            if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0)
               error("ERROR opening socket");
            bzero((char *) &serv_addr, sizeof(serv_addr));
            portno = atoi(argv[1]);
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(portno);
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
               error("ERROR on binding");
            listen(sockfd,5);
            clilen = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0)
                 error("ERROR on accept");
            bzero(buffer,256);
            n = read(newsockfd,buffer,255);
            if (n < 0) error("ERROR reading from socket");
            printf("Here is the message: %s",buffer);
            n = write(newsockfd,"I got your message",18);
            if (n < 0) error("ERROR writing to socket");
            signal(SIGCHLD,SigCatcher);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else
            close(newsockfd);
     } /* end of while */
         }
         // sockfdudp is read ready
         if(FD_ISSET(sockfdudp, &readfds))
         {
           // do udp portion
            if (argc < 2) {
            fprintf(stderr, "ERROR, no port provided\n");
            exit(0);
         }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   while (1) {
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       write(1,"Received a datagram: ",21);
       write(1,buf,n);
       n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
         }
     }
     //**//
   
     /*newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     newsockfdudp = accept(sockfdudp, (struct sockaddr *) &cli_addr, &clilen2);
     if (newsockfd < 0)
      error("ERROR on accept");
     if (newsockfdudp < 0)
      error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) 
        error("ERROR reading from socket");
     printf("Here is the message: %s",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) 
        error("ERROR writing to socket");
     //Causes a nonblocking wait to prevent Zombie processes
     signal(SIGCHLD,SigCatcher);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         newsockfdudp = accept(sockfdudp, 
               (struct sockaddr *) &cli_addr, &clilen2);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else {
            close(sockfdudp);
            dostuff(newsockfdudp);
            exit(0);
         }*/
         close(newsockfd);
         close(newsockfdudp);
     } /* end of while */
     close(sockfd);
     close(sockfdudp);
     return 0; /* we never get here */
}


