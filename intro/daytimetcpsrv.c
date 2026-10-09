#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "unp.h"
#include "error.h"

int main(int argc, char **argv) {
  int listenfd, connfd;
  char buff[MAXLINE];
  struct sockaddr_in servaddr;
  time_t ticks;

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_sys("socket error");

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(13);

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    err_sys("bind error");

  if (listen(listenfd, LISTENQ) < 0)
    err_sys("listen error");

  for (;;) {
    if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0)
      err_sys("accept error");

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

    if (write(connfd, buff, strlen(buff)) < 0)
      err_sys("write error");

    if (close(connfd) < 0)
      err_sys("close error");
  }
  exit(0);
}