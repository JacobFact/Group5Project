#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
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
