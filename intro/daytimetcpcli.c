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
  struct sockaddr_in servaddr; /* sockaddr_in defined in <netinet/in.h> */

  if (argc != 2)
    err_quit("usage: %s <IPaddress>", argv[0]);

  /* socket(int address_family, int socket_type, int protocol)
   * defined in <sys/socket.h>
   *   address_family: Says what kind of addresses this socket will speak, which
   *                   fixes the shape of the address struct you'll hand to
   *                   `connect`/`bind`. `AF_INET` → IPv4 → `struct sockaddr_in`
   *                    (32-bit address + 16-bit port)
   *   socket_type: SOCK_STREAM: reliable ordered byte stream (TCP)
   *                SOCK_DGRAM: unreliable, unordered, boundary-preserving
   *                             datagrams (UDP)
   *                SOCK_SEQPACKET: reliable, ordered, and boundary-preserving
   *                                (SCTP)
   *                SOCK_RAW:  direct IP access; requires root
   *   protocol: 0: "the default for this pair"
   *                AF_INET + SOCK_STREAM → IPPROTO_TCP;
   *                AF_INET + SOCK_DGRAM → IPPROTO_UDP
   */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_sys("socket error");

  memset(&servaddr, 0, sizeof(servaddr)); /* defined in <string.h> */
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(13);
  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <=
      0) /* defined in <arpa/inet.h> */
    err_quit("inet_pton error for %s", argv[1]);

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    err_sys("connect error");

  while ((n = read(sockfd, recvline, MAXLINE)) >
         0) {        /* read() defined in <unistd.h> */
    recvline[n] = 0; /* null terminate */
    if (fputs(recvline, stdout) == EOF)
      err_sys("fputs error");
  }
  if (n < 0)
    err_sys("read error");

  exit(0);
}