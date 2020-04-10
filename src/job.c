#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

#include "unit.h"

#define GET_TIME 335
#define PRINT_TIME 336

int main(int argc, const char **argv) {
  if (argc != 3) {
    fprintf(stderr, "[Usage] ./job process-name execution-time\n");
    exit(1);
  }
  printf("%s %d\n", argv[1], (int)getpid());
  struct timespec st, ed;
  syscall(GET_TIME, &st);
  unsigned exec_time = (unsigned)atoi(argv[2]);
  Sleep(exec_time);
  syscall(GET_TIME, &ed);
  syscall(PRINT_TIME, getpid(), &st, &ed);
  return 0;
}
