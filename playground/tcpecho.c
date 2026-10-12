#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "unp.h"
#include "tcpecho.h"
#include "error.h"

static void str_echo(int);
static void str_cli(int);

void server() {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr, srvaddr;

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_sys("socket error");

  memset(&srvaddr, 0, sizeof(srvaddr));
  srvaddr.sin_family = AF_INET;
  srvaddr.sin_port = htons(8080);
  srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listenfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
    err_sys("bind error");

  if (listen(listenfd, LISTENQ) < 0)
    err_sys("listen error");

  for (;;) {
    clilen = sizeof(cliaddr);
    if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
      err_sys("accept error");

    if ((childpid = fork()) < 0)
      err_sys("fork error");
    else if (childpid == 0) {
      close(listenfd);
      str_echo(connfd);
      close(connfd);
      exit(0);
    }

    close(connfd);
  }
  close(listenfd);
  exit(0);
}

void client(char *ip) {
  int sockfd;
  struct sockaddr_in srvaddr;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_sys("socket error");

  memset(&srvaddr, 0, sizeof(srvaddr));
  srvaddr.sin_family = AF_INET;
  srvaddr.sin_port = htons(8080);

  if (inet_pton(AF_INET, ip, &srvaddr.sin_addr) <= 0)
    err_sys("inet_pton error");

  if (connect(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
    err_sys("connect error");

  str_cli(sockfd);

  close(sockfd);
  exit(0);
}

void str_echo(int connfd) {
  ssize_t rc;
  char buf[MAXLINE];
  char srv_ip[INET_ADDRSTRLEN], cli_ip[INET6_ADDRSTRLEN];
  socklen_t srvlen, clilen;
  struct sockaddr_in srvaddr, cliaddr;

  srvlen = sizeof(srvaddr);
  if (getsockname(connfd, (struct sockaddr *)&srvaddr, &srvlen) < 0)
    err_sys("str_echo: getsockname error");

  clilen = sizeof(cliaddr);
  if (getpeername(connfd, (struct sockaddr *)&cliaddr, &clilen) < 0)
    err_sys("str_echo: getpeername error");

  if (inet_ntop(AF_INET, &srvaddr.sin_addr, srv_ip, sizeof(srvaddr)) == NULL)
    err_sys("str_echo: srvaddr inet_ntop error");

  if (inet_ntop(AF_INET, &cliaddr.sin_addr, cli_ip, sizeof(cliaddr)) == NULL)
    err_sys("str_echo: cliaddr inet_ntop error");

  snprintf(buf, sizeof(buf), "Connected to %s. Client IP: %s\r\n", srv_ip, cli_ip);
  if (writen(connfd, buf, strlen(buf)) < 0)
    err_sys("str_echo: welcome msg: writen error");

  while ((rc = read(connfd, buf, sizeof(buf))) > 0)
    if (writen(connfd, buf, rc) < 0)
      err_sys("str_echo: echo write: writen error");
  
  if (rc < 0)
    err_sys("str_echo: read error");
}

void str_cli(int sockfd) {
  int rc;
  char recvline[MAXLINE], sendline[MAXLINE];

  if ((rc = readline(sockfd, recvline, sizeof(recvline))) < 0)
    err_sys("str_cli: welcome msg: readline error");
  else if (rc > 0)
    if (fputs(recvline, stdout) == EOF)
      err_sys("str_cli: welcome msg: fputs error");

  printf("> ");
  fflush(stdout);

  while (fgets(sendline, sizeof(sendline), stdin) != NULL) {
    if (writen(sockfd, sendline, strlen(sendline)) < 0)
      err_sys("str_cli: echo write: writen error");

    if ((rc = readline(sockfd, recvline, sizeof(recvline))) < 0)
      err_sys("str_cli: echo read: readline error");
    else if(rc == 0)
      return;
    
    if (fputs(recvline, stdout) == EOF)
      err_sys("str_cli: echo response: fputs error");

    printf("\n> ");
    fflush(stdout);
  }
}