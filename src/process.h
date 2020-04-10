#ifndef PROCESS_H_
#define PROCESS_H_

#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_NAME_LENGTH 40

#define RUNNING 0
#define WAITING 1

typedef struct {
  char name[MAX_NAME_LENGTH];
  unsigned ready_time;
  unsigned exec_time;
  unsigned running_time;
  pid_t pid;
  int status;
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

int RunProcess(Process *process) {
  int pri = sched_get_priority_max(SCHED_FIFO);
  if (pri < 0) return -1;
  printf("run pri = %d\n", pri);
  struct sched_param param;
  param.sched_priority = pri;
  if (sched_setscheduler(process->pid, SCHED_FIFO, &param) < 0) {
    perror("RunProcess - sched_setscheduler: ");
    exit(1);
  }
  process->status = RUNNING;
  return 0;
}

int PauseProcess(Process *process) {
  int pri = sched_get_priority_min(SCHED_FIFO);
  if (pri < 0) return -1;
  printf("pause pri = %d\n", pri);
  struct sched_param param;
  param.sched_priority = pri;
  if (sched_setscheduler(process->pid, SCHED_FIFO, &param) < 0) {
    perror("PauseProcess - sched_setscheduler: ");
    exit(1);
  }
  process->status = WAITING;
  return 0;
}

void ForkProcess(Process *process) {
  pid_t pid;
  if ((pid = fork()) < 0) {
    fprintf(stderr, "[Error] Failed to fork().\n");
    exit(1);
  }
  if (pid == 0) {
    struct sched_param param;
    param.sched_priority = sched_get_priority_min(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &param) < 0) {
      perror("sched_setscheduler");
      exit(1);
    }
    char buf[10];
    snprintf(buf, 10, "%u", process->exec_time);
    execl("./job", "./job", process->name, buf, NULL);
  } else {
    process->pid = pid;
    process->status = WAITING;
  }
}

#endif  // PROCESS_H_
