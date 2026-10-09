#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "unp.h"
#include "error.h"

char *sock_ntop(const struct sockaddr *sa, socklen_t salen) {
  char portstr[8]; /* 2bytes for ':]', 5bytes for '65535', and 1byte for '\0' */

  static char str[INET6_ADDRSTRLEN + 8]; /* IPv6 is largest */
  /* INET6_ADDRSTRLEN is 46bytes including '\0' */
  /* But as we include \0 in portstr as well, */
  /* this extra byte is used for starting '[' in IPv6*/

  switch (sa->sa_family) {
  case AF_INET: {
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;

    if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
      return NULL;
    if (ntohs(sin->sin_port) != 0) {
      snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
      strcat(str, portstr);
    }
    return str;
  }
  case AF_INET6: {
    struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;

    str[0] = '[';
    if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
      return NULL;
    if (ntohs(sin6->sin6_port) != 0) {
      snprintf(portstr, sizeof(portstr), "]:%d", ntohs(sin6->sin6_port));
      strcat(str, portstr);
    }
    return str;
  }
  default: {
    snprintf(str, sizeof(str), "sock_ntop: nknown AF_xxx: %d, len %d",
             sa->sa_family, salen);
    return str;
  }
  }
  return NULL;
}

char *Sock_ntop(const struct sockaddr *sa, socklen_t salen) {
  char *ptr;

  if ((ptr = sock_ntop(sa, salen)) == NULL)
    err_sys("sock_ntop error");
  return ptr;
}