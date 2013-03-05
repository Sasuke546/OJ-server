#ifndef SOCSERVER
#define SOCSERVER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define LISTENQ	20
#define MAXLINE	40
#define BUFFERSIZE	4096
#define PORT	3336

ssize_t readn(int fd, char *vptr, size_t n);
ssize_t writen(int fd, const char * vptr, size_t n);
ssize_t readline(int fd, char * vptr, size_t maxlen);
int Socket(int domain, int type, int protocol);
int Accept(int sockfd, struct sockaddr * addr, socklen_t * addrlen);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Close(int fd);
void Inet_pton(int family, const char *strptr, void *addrptr);
int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);

#endif