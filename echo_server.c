////www.taywils.me/2014/11/09/udpechoinc_p2.html
//http://www.paulgriffiths.net/program/c/echoserv.php
//http://www.linuxhowtos.org/C_C++/socket.htm
// http://www.cplusplus.com/forum/unices/116977/
//https://www.ibm.com/developerworks/linux/tutorials/l-sock/index.html
// What is the signficance of port numbers in making a connection?
//“echo_s”	should	be	able	to	handle	multiple	clients,	by	creating	multiple	processes. Do we already handle that with the way
// we fork proceeses for each connection we get?
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "server_functions.h"
int main(int argc, char *argv[])
{



   int sockfd, newsockfd, portno, sockfd_udp, length, n;
   int portno2 =2017; // default value;
   int portno3 = 2018;
   //sockfd and newsockfd are file descriptors, i.e. array subscripts into the file descriptor table . 
   // These these "sock variables" store the values returned by the socket system call and the accept system call.
   // the "portno variables"  stores the port number/s on which the server accepts connections
   // n is the return value for the read() and write() calls; i.e. it contains the number of characters read or written.
     socklen_t clilen; // clilen stores the size of the address of the client. This is needed for the accept system call.
     char buffer[256]; // The server reads characters from the socket connection into this buffer. May store message
     struct sockaddr_in serv_addr, cli_addr; // Make multiple of them to accept multiple port numbers
   struct sockaddr_in serv_addr2; // for port number 2
   struct sockaddr_in serv_addr3; // for port number 3
     socklen_t fromlen; //???
     // // A sockaddr_in is a structure containing an internet address. This structure is defined in netinet/in.h.
      struct sockaddr_in server; // should this be serv_addr?
     struct sockaddr_in from; // should this be cli_addr?
   
   // The variable serv_addr(serverr) will contain the address of the server, and cli_addr(from) will contain the address of the client which connects to the server.


     char buf[1024]; // The server reads characters from the socket connection into this buffer. May store message
     fd_set readfds; // ???

     // outputs an error if no port is provided
     if (argc < 2){
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     // creates a new TCP and UDP socket. I assume input validation is happening in those functions
     sockfd = makeSocket(SOCK_STREAM);
     sockfd_udp = makeSocket(SOCK_DGRAM);

     // sets the values in serv_addr
   //The function bzero() sets all values in a buffer to zero. It takes two arguments, 
   //the first is a pointer to the buffer and the second is the size of the buffer. Thus, this line initializes serv_addr to zeros.
     bzero((char *) &serv_addr, sizeof(serv_addr)); // when does serv_addr store the buffer? 
     
    // Getting port numbers
   //The port number on which the server will listen for connections is passed in as an argument,
   //and this statement uses the atoi() function to convert this from a string of digits to an integer.
   // “echo_s” should	reply	to	any	client client	connected	to	one	of	its	ports	with	the	same	
   // message	it	receives.
   portno = atoi(argv[1]); // How do you know if this port number is the same or from the client port number
      
   if(argc > 2)
     {
        portno2 = atoi(argv[2]);
     } 
        if(argc > 3)
     {
        portno3 = atoi(argv[3]);
     }

   
   //The variable serv_addr is a structure of type struct sockaddr_in. This structure has four fields. The first field is 
   //short sin_family, which contains a code for the address family. It should always be set to the symbolic constant AF_INET.
     serv_addr.sin_family = AF_INET;
  
     serv_addr.sin_addr.s_addr = INADDR_ANY;
   //The third field of sockaddr_in is a structure of type struct in_addr which contains only a single field unsigned long s_addr. 
   // This field contains the IP address of the host. For server code, this will 
   // always be the IP address of the machine on which the server is running, and there is a 
   // symbolic constant INADDR_ANY which gets this address.
   
   // How to get IP addresses of machine where client is running rather than the the machine the server is running?
     
   serv_addr.sin_port = htons(portno); // assigns serv_addr the port number?
   // The second field of serv_addr is unsigned short sin_port, which contain the port number. However, instead of simply copying 
   // the port number to this field, it is necessary to convert this 
   // to network byte order using the function htons() which converts a port number in host byte order to a port number 
   // in network byte order.

   // Setting up for port 2
        serv_addr2.sin_family = AF_INET; 
  
     serv_addr2.sin_addr.s_addr = INADDR_ANY;
  
     
   serv_addr2.sin_port = htons(portno2); 
   
      serv_addr3.sin_family = AF_INET; 
  
     serv_addr3.sin_addr.s_addr = INADDR_ANY;
   
      // Setting up for port 3
        serv_addr2.sin_family = AF_INET; 
  
     serv_addr2.sin_addr.s_addr = INADDR_ANY;
  
     
   serv_addr2.sin_port = htons(portno2); 
   
      serv_addr3.sin_family = AF_INET; 
  
     serv_addr3.sin_addr.s_addr = INADDR_ANY;
  
     
   serv_addr3.sin_port = htons(portno3); 
     // binds each socket to the port and outputs an error if it fails
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
         error("ERROR on binding");
     if (bind(sockfd_udp, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
         error("ERROR on binding");
   
   //Bind the socket to an address using the bind() system call. 
   //For a server socket on the Internet, an address consists of a port number on the host machine.

     // listens to each socket and outputs an error if it fails
     listen(sockfd,5);
    int  clilen = sizeof(cli_addr); // stores the size of the address of the client
     listen(sockfd_udp,5); 
    int  clilen2 = sizeof(cli_addr); // I do not know the use of this?
   
   // What if multiple clients connect to the server? Can make multiple proceeses to handle this?

     // clear the set
   // Why clear the set? What does it even do? 

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
            n = read(newsockfd,buffer,255); // but sockfd not explicity assigned message from client
            if (n < 0)
               error("ERROR reading from socket");

            // outputs the message received from the client to the standard output
            printf("Here is the message: %s",buffer);

            // sends a message to the client, outputs an error if it fails
            n = write(newsockfd,"I got your message",18);
            if (n < 0)
               error("ERROR writing to socket");

            // non-blocking wait for zombie processes
            signal(SIGCHLD,SigCatcher);

            // continuous server that creates a child process
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
            // This call typically blocks until a client connects with the server.
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
            signal(SIGCHLD,SigCatcher);

            // continuous server that creates a child process
           
            while (1)
            {
               // accepts the TCP socket and outputs an error if it fails
               newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen); //(struct sockaddr *)
               if (newsockfd < 0)
                  error("ERROR on accept");

               // creates a child process and outputs an error if it fails
               pid_t  pid = fork();
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
            signal(SIGCHLD,SigCatcher);

            // continuous server that creates a child process




 while (1) {
               // creates a child process and outputs an error if it fails
              pid_t  pid2 = fork();
               if (pid2 < 0) // I am renaming the snd pid pid2 to be careful
                  error("ERROR on fork");

               // the child process
               if (pid2 == 0)
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
    // https://www.cs.bu.edu/teaching/c/file-io/intro/
    /*FILE *ifp, *ofp;
   char *mode = "r";
   char outputFilename[] = "echo.list";

   ifp = fopen("in.list", mode);

   if (ifp == NULL) {
     fprintf(stderr, "Can't open input file in.list!\n");
     exit(1);
   }

   ofp = fopen(outputFilename, "w");

   if (ofp == NULL) {
     fprintf(stderr, "Can't open output file %s!\n",
             outputFilename);
     exit(1);
   }
   // Once a file has been successfully opened, you can read from it using fscanf() or write to it using fprintf();
   fclose(ifp);
fclose(ofp);
        
