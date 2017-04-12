SHELL = /bin/sh

FLAGS = -Wall -lsocket -o

CC = gcc


all:  client client_functions server server_functions


client:			client.o client_functions.o
			gcc -o client client.o client_functions.o

client.o:		client.c client_functions.h
			gcc -c client.c
			
client_functions.o:	client_functions.c
			gcc -c client_functions.c

server:			server.o server_functions.o
			gcc -o server server.o server_functions.o

server.o: 		server.c server_functions.h
			gcc -c server.c
			
server_functions.o:	server_functions.c
			gcc -c server_functions.c

clean:  
			rm -f client client.o client_functions client_functions.o server server.o server_functions server_functions.o
