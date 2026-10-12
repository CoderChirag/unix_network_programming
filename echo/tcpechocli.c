#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "unp.h"
#include "error.h"

void str_cli(FILE *, int);

int main(int argc, char **argv){
  int sockfd;
  struct sockaddr_in srvaddr;

  if (argc != 2)
    err_quit("usage: %s <IPaddress>", argv[0]);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_sys("socket error");

  memset(&srvaddr, 0, sizeof(srvaddr));
  srvaddr.sin_family = AF_INET;
  srvaddr.sin_port = htons(8080);
  if (inet_pton(AF_INET, argv[1], &srvaddr.sin_addr) <= 0)
    err_sys("inet_pton error for %s", argv[1]);

  if (connect(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
    err_sys("connect error");

  str_cli(stdin, sockfd);

  if (close(sockfd) < 0)
    err_sys("close error");

  exit(0);
}

void str_cli(FILE *fp, int sockfd) {
  ssize_t rc;
  char sendline[MAXLINE], recvline[MAXLINE];

  if ((rc = readline(sockfd, recvline, sizeof(recvline))) < 0)
    err_sys("str_cli: readline error");
  else if (rc == 0){
    close(sockfd);
    err_quit("str_cli: server terminated prematurely");
  }

  if (fputs(recvline, stdout) == EOF)
    err_sys("str_cli: fputs error");

  if (fputs("> ", stdout) == EOF)
    err_sys("str_cli: fputs error");

  while(fgets(sendline, MAXLINE, fp) != NULL) {
    if (writen(sockfd, sendline, strlen(sendline)) < 0)
      err_sys("str_cli: writen error");
    
    if ((rc = readline(sockfd, recvline, sizeof(recvline))) < 0)
      err_sys("str_cli: readline error");
    else if (rc == 0){
      close(sockfd);
      err_quit("str_cli: server terminated prematurely");
    }

    if (fputs(recvline, stdout) == EOF)
      err_sys("str_cli: fputs error");
    
    if (fputs("\n> ", stdout) == EOF)
      err_sys("str_cli: fputs error");
  }
}