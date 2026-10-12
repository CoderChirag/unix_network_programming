#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "unp.h"

void inet() {
  struct sockaddr_in sin, broad;
  struct sockaddr_in6 sin6, broad6;

  memset(&sin, 0, sizeof(sin));
  memset(&sin6, 0, sizeof(sin6));
  memset(&broad, 0, sizeof(broad));
  memset(&broad6, 0, sizeof(broad6));

  sin.sin_family = AF_INET;
  sin.sin_port = htons(8080);
  sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  broad.sin_family = AF_INET;
  broad.sin_port = htons(65535);
  broad.sin_addr.s_addr = htonl(INADDR_BROADCAST);

  sin6.sin6_family = AF_INET6;
  sin6.sin6_port = htons(80);
  sin6.sin6_addr = in6addr_loopback;

  broad6.sin6_family = AF_INET6;
  broad6.sin6_port = htons(65535);
  memset(&broad6.sin6_addr, 0xff, sizeof(broad6.sin6_addr));

  printf("IPv4: %s\n", Sock_ntop((struct sockaddr *)&sin, sizeof(sin)));
  printf("IPv4: %s\n", Sock_ntop((struct sockaddr *)&broad, sizeof(broad)));
  printf("IPv6: %s\n", Sock_ntop((struct sockaddr *)&sin6, sizeof(sin6)));
  printf("IPv6: %s\n", Sock_ntop((struct sockaddr *)&broad6, sizeof(broad6)));
}