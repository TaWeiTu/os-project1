#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_NAME_LENGTH 40

typedef struct {
  char name[MAX_NAME_LENGTH];
  unsigned ready_time;
  unsigned exec_time;
  unsigned running_time;
  pid_t pid;
} Process;

int CompareProcess(const void *a, const void *b) {
  Process *p1 = (Process *)a;
  Process *p2 = (Process *)b;
  if (p1->ready_time != p2->ready_time) {
    return p1->ready_time < p2->ready_time ? -1 : 1;
  }
  if (p1->exec_time != p2->exec_time) {
    return p1->exec_time < p2->exec_time ? -1 : 1;
  }
  return 0;
}

void ForkProcess(Process *process) {
  pid_t pid;
  if ((pid = fork()) < 0) {
    fprintf(stderr, "[Error] Failed to fork().\n");
    exit(1);
  }
  if (pid == 0) {
    char buf[10];
    snprintf(buf, 10, "%u", process->exec_time);
    execl("./job", "./job", process->name, buf, NULL);
  }
  process->pid = pid;
}

#endif  // PROCESS_H_
