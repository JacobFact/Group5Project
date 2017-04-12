// client_functions.c
// contains the common functions of the client.c file

#include <std.io>

// the error function prints the error message that is passed as an argument to it
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
