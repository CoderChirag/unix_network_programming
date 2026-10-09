#include "../config.h"
#include <sys/socket.h>

#ifndef UNP_H
#define UNP_H

#define MAXLINE 4096

#define LISTENQ 1024

char *sock_ntop(const struct sockaddr *sockaddr, socklen_t addrlen);
char *Sock_ntop(const struct sockaddr *sockaddr, socklen_t addrlen);

ssize_t readn(int fd, void *buff, size_t nbytes);
ssize_t writen(int fd, const void *buff, size_t nbytes);

ssize_t readline(int fd, void *buff, size_t maxlen);
ssize_t readlinebuf(void **buffptr);

#endif