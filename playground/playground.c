#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

#include "unp.h"
#include "tcpecho.h"
#include "error.h"

int main(int argc, char **argv) {

  if (argc < 2)
    err_quit("usage: %s server|client", argv[0]);

  if (strcmp(argv[1], "server") == 0)
    server();
  else if (strcmp(argv[1], "client") == 0) {
    if (argc != 3)
      err_quit("usage: %s client <IPaddress>", argv[0]);
    client(argv[2]);
  }else {
    err_quit("usage: %s server|client", argv[0]);
  }

  return 0;
}