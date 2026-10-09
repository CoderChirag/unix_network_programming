#include <unistd.h>
#include <errno.h>

#include "unp.h"

/* Read n bytes of data from a file descriptor */
ssize_t readn(int fd, void *buff, size_t nbytes) {
  size_t nleft;
  ssize_t nread;
  char *ptr;

  ptr = buff;
  nleft = nbytes;
  while (nleft > 0) {
    if ((nread = read(fd, ptr, nleft)) < 0) {
      if (errno == EINTR)
        nread = 0; /* and call read() again */
      else
        return -1;
    } else if (nread == 0)
      break; /* EOF */

    nleft -= nread;
    ptr += nread;
  }

  return (nbytes - nleft); /* return >= 0 */
}
