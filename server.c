/* a server in the unix domain.  The pathname of 
   the socket address is passed as an argument */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
void error(const char *);
void *SigCatcher(int n)
{
  wait3(NULL,WNOHANG,NULL);
}
int main(int argc, char *argv[])
{
   int sockfd, newsockfd, servlen, n;
   socklen_t clilen;
   struct sockaddr_un  cli_addr, serv_addr;
   char buf[80];

   //Create socket with validation
   if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0){
      error("Socket Creation Failed: Exiting...");
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sun_family = AF_UNIX;
   strcpy(serv_addr.sun_path, argv[1]);
   servlen=strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
   
   if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0){
      error("Binding Socket Failed: Exiting..."); 
   }

   listen(sockfd,5);
   clilen = sizeof(cli_addr);
     //Causes a nonblocking wait to prevent Zombie processes
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
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
      
   //read from client
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0){
      error("ERROR reading from socket");
   }
   //Display the message
   printf("Here is the message: %s\n",buffer);
   ///Reply
   n = write(sock,"I got your message",18);
   if (n < 0){
       error("ERROR writing to socket");
   }
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
