#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "unp.h"
#include "error.h"

int main(int argc, char **argv) {
  int sockfd, n;
  char recvline[MAXLINE + 1];
  struct sockaddr_in6 servaddr;

  if (argc != 2)
    err_quit("usage: %s <IPaddress>", argv[1]);

  if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
    err_sys("socket error");

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin6_family = AF_INET6;
  servaddr.sin6_port = htons(13);
  if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0)
    err_sys("inet_pton error");

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    err_sys("connect error");

  while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;
    if (fputs(recvline, stdout) == EOF)
      err_sys("fputs error");
  }

  if (n < 0)
    err_sys("read error");
  exit(0);
}