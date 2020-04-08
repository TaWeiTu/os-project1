#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "unit.h"

int main(int argc, const char **argv) {
  if (argc != 3) {
    fprintf(stderr, "[Usage] ./job process-name execution-time\n");
    exit(1);
  }
  printf("%s %d\n", argv[1], (int)getpid());
  unsigned exec_time = (unsigned)atoi(argv[2]);
  Sleep(exec_time);
  return 0;
}
