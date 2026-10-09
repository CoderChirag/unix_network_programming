#include <unistd.h>
#include <errno.h>

#include "unp.h"

static int read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];

static ssize_t read_byte(int fd, char *ptr) {
  if (read_cnt <= 0) {
  again:
    if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
      if (errno == EINTR)
        goto again;
      return -1;
    } else if (read_cnt == 0)
      return 0;
    read_ptr = read_buf;
  }

  read_cnt--;
  *ptr = *(read_ptr++);
  return 1;
}

ssize_t readline(int fd, void *buff, size_t maxlen) {
  size_t n;
  ssize_t rc;
  char *ptr;

  ptr = buff;
  for (n = 0; n < maxlen - 1;) {
    if ((rc = read_byte(fd, ptr)) < 0)
      return -1; /* error, errno set by read() */
    else if (rc == 0)
      break; /* EOF, n-1 bytes were read */
    n++;
    if (*(ptr++) == '\n')
      break; /* newline is stored, like fgets() */
  }

  *ptr = 0; /* null terminate like fgets() */
  return n;
}

ssize_t readlinebuf(void **buffptr) {
  if (read_cnt)
    *buffptr = read_ptr;
  return read_cnt;
}