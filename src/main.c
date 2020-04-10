#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"
#include "schedule.h"
#include "unit.h"

static int ReadPolicy() {
  char policy[10];
  scanf("%s", policy);
  if (strcmp(policy, "FIFO") == 0) return FIFO;
  if (strcmp(policy, "RR") == 0) return RR;
  if (strcmp(policy, "SJF") == 0) return SJF;
  if (strcmp(policy, "PSJF") == 0) return PSJF;
  return -1;
}

static void SetAffinity() {
  pid_t pid = getpid();
  cpu_set_t mask;
  sched_getaffinity(pid, sizeof(cpu_set_t), &mask);
  cpu_set_t set_mask;
  CPU_ZERO(&set_mask);
  CPU_SET(0, &set_mask);
  sched_setaffinity(pid, sizeof(cpu_set_t), &set_mask);
  sched_getaffinity(pid, sizeof(cpu_set_t), &mask);
}

int main() {
  int policy = ReadPolicy();
  if (policy < 0) {
    fprintf(stderr, "[Error] Unknown policy.\n");
    exit(1);
  }
  SetAffinity();
  unsigned num_process;
  scanf("%u", &num_process);
  Process *process = (Process *)malloc(num_process * sizeof(Process));
  for (unsigned i = 0; i < num_process; ++i) {
    scanf("%s%u%u", process[i].name, &process[i].ready_time,
          &process[i].exec_time);
    process[i].running_time = 0;
  }
  qsort(process, num_process, sizeof(Process), CompareProcess);
  if (Schedule(process, num_process, policy) < 0) {
    fprintf(stderr, "[Error] Failed to schedule processes.\n");
  }
  free(process);
  return 0;
}
