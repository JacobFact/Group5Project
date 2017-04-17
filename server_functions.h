// server_functions.h
// contains the declarations of the functions used in the server.c file

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>

// declaration of the dostuff function
void dostuff(int);

// declaration of the error function
void error(const char*);

// declaration of the SigCatcher function
void *SigCatcher(int);

// declaration of the makeSocket function
int makeSocket(int);
