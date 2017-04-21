all:  client server


client:			client.o client_functions.o
			gcc -o client client.o client_functions.o

client.o:		
			gcc -c client.c
			
client_functions.o:	client_functions.h
			gcc -c client_functions.c

server:			server.o server_functions.o
			gcc -o server server.o server_functions.o

server.o: 		
			gcc -c server.c
			
server_functions.o:	server_functions.h
			gcc -c server_functions.c

clean:  
			rm -f client client.o client_functions client_functions.o server server.o server_functions server_functions.o
