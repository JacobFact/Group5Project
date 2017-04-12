/* a client in the unix domain */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
void error(const char *);

int main(int argc, char *argv[])
{
   //sockfd: stores socket system call values|servlen: stores the size of the address of the server|n: stores the return values of read() and write() calls
   int sockfd, servlen,n;
   //sockaddr_un: structure containing an port address for TCP and/or UCP|serv_addr: contains the address of the server
   struct sockaddr_un  serv_addr;
   //Server reads character from the socket connection into this buffer
   char buffer[82];

   //Sets all values in the buffer to zero
   bzero((char *)&serv_addr,sizeof(serv_addr));
   
   //Contains code for the address family, UNIX domain
   serv_addr.sun_family = AF_UNIX;
   
   //Copies the string (port number(?)) from the command line to serv_addr.sun_path
   strcpy(serv_addr.sun_path, argv[1]);
   
   //Stores the server address
   servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
   
   //creates a new socket with the Unix address domain, stream type, and protocal message
   if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0){
      //Error message if socket call fails
      error("Socket Call Fail: Exiting...");
   }
   
   //connect to server
   if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0){
      //Error Message
      error("Connection Failed: Exiting...");
   }
   
   //Message input
   printf("Please enter your message: ");
   bzero(buffer,82);
   //Reads from stdin
   fgets(buffer,80,stdin);
   //Writes message to socket
   write(sockfd,buffer,strlen(buffer));
   //Reads reply from the socket
   n=read(sockfd,buffer,80);
   //Display
   printf("The return message was\n");
   write(1,buffer,n);
   close(sockfd);
   return 0;
}

//Displays Error message if call fails
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
