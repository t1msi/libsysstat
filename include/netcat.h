/*
 * https://github.com/guzlewski/netcat
 */

#ifndef _NETCAT_H
#define _NETCAT_H

#define MAXBUF 65536
#define BACKLOG 1

typedef struct args
{
    int src;
    int dest;
} args;

void *Thread(void *arg);
int CreateClientSocket(char *address, char *port, int *type, int *family);
void Client(char *address, char *port, int *type, int *family);
int CreateServerSocket(char *address, char *port, int *type, int *family);
void Server(char *address, char *port, int *type, int *family);
void PrintError(char *function);
void Help(char *name);

#endif /* _NETCAT_H */

