#include <unistd.h>
#include <errno.h>

#include "unp.h"

/* Write n bytes of data to a file descriptor */
ssize_t writen(int fd, const void *buff, size_t nbytes) {
  ssize_t nwritten;
  size_t nleft;
  const char *ptr;

  ptr = buff;
  nleft = nbytes;
  while (nleft > 0) {
    if ((nwritten = write(fd, ptr, nleft)) <= 0) {
      if (nwritten < 0 && errno == EINTR)
        nwritten = 0; /* and call write() again */
      else
        return -1; /* error */
    }

    nleft -= nwritten;
    ptr += nwritten;
  }

  return nbytes;
}
