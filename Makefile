SHELL = /bin/sh

FLAGS = -Wall -lsocket -o

CC = gcc


all:  client client_functions server server_functions



client:			client.c
                  
			$(CC) $(FLAGS) $@ client.c



client_functions: 	client_functions.c
                  
			$(CC) $(FLAGS) $@ client_functions.c



server: 		server.c
                  
			$(CC) $(FLAGS) $@ server.c



server_functions:	server_functions.c
                  
			$(CC) $(FLAGS) $@ server_functions.c



clean:  rm client client_functions server server_functions
