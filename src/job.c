#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "unit.h"

int main(int argc, const char **argv) {
  if (argc != 3) {
    fprintf(stderr, "[Usage] ./job process-name execution-time\n");
    exit(1);
  }
  struct timeval st, ed;
  gettimeofday(&st, NULL);
  printf("%s %d\n", argv[1], (int)getpid());
  unsigned exec_time = (unsigned)atoi(argv[2]);
  Sleep(exec_time);
  gettimeofday(&ed, NULL);
  fprintf(stderr, "[Project 1] %d %ld.%06ld %ld.%06ld\n", (int)getpid(),
          st.tv_sec, st.tv_usec, ed.tv_sec, ed.tv_usec);
  return 0;
}
