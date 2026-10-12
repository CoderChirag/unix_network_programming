#include <unistd.h>

#include "unp.h"
#include "error.h"

pid_t Fork(void) {
  pid_t pid;

  if ((pid = fork()) == -1)
    err_sys("fork error");

  return pid;
}