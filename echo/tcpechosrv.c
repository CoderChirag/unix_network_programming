#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "unp.h"
#include "error.h"

void str_echo(int, const char *);

int main() {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr, srvaddr;

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_sys("socket error");

  memset(&srvaddr, 0, sizeof(srvaddr));
  srvaddr.sin_family = AF_INET;
  srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  srvaddr.sin_port = htons(8080);

  if(bind(listenfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
    err_sys("bind error");

  if(listen(listenfd, LISTENQ) < 0)
    err_sys("listen error");

  for (;;) {
    clilen = sizeof(cliaddr);
    if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
      err_sys("accept error");

    if ((childpid = Fork()) == 0) {
      struct sockaddr_in connaddr;
      socklen_t connaddr_len;
      char ip[INET_ADDRSTRLEN];

      if (close(listenfd) < 0)
        err_sys("child: listenfd close error");

      connaddr_len = sizeof(connaddr);
      if (getsockname(connfd, (struct sockaddr *)&connaddr,
                      &connaddr_len) < 0)
        err_sys("getsockname error");
      
      inet_ntop(AF_INET, &connaddr.sin_addr.s_addr, ip, sizeof(ip));
      str_echo(connfd, ip);
      exit(0);
    }

    if (close(connfd) < 0)
      err_sys("main: connfd close error");
  }

  if (close(listenfd) < 0)
    err_sys("main: listenfd close error");

  exit(0);
}

void str_echo(int connfd, const char *ip) {
  ssize_t rc;
  char buff[MAXLINE], welcome_txt[MAXLINE];

  snprintf(welcome_txt, sizeof(welcome_txt), "Connected to %s\r\n", ip);
  if(writen(connfd, welcome_txt, strlen(welcome_txt)) < 0)
    err_sys("str_echo: welcome message: writen error");

again:
  while ((rc = read(connfd, buff, sizeof(buff))) > 0){
    if (writen(connfd, buff, rc) < 0)
      err_sys("str_echo: writen error");
  }
  
  if (rc < 0 && errno == EINTR) 
    goto again;
  else if (rc < 0)
    err_sys("str_echo: read error");

  if (close(connfd) < 0)
    err_sys("str echo: connfd close error");
}