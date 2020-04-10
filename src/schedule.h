#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <assert.h>
#include <sys/wait.h>

#include "process.h"
#include "queue.h"
#include "unit.h"

#define FIFO 0
#define RR 1
#define SJF 2
#define PSJF 3

static int ScheduleFIFO(Process *process, unsigned num_process) {
  ProcessQueue que = ConstructQueue();
  unsigned ptr = 0;
  for (unsigned t = 0; ptr < num_process || que.size > 0; t++) {
    while (ptr < num_process && process[ptr].ready_time == t) {
      printf("Fork %s\n", process[ptr].name);
      ForkProcess(&process[ptr]);
      EnQueue(&que, &process[ptr]);
      ptr++;
    }
    while (que.size > 0) {
      const Process *p = GetFront(&que);
      if (p->running_time < p->exec_time) break;
      printf("Terminate %s\n", p->name);
      waitpid(p->pid, NULL, 0);
      DeQueue(&que);
    }
    if (que.size > 0) {
      if (GetFront(&que)->running_time == 0) {
        printf("Run %s\n", GetFront(&que)->name);
        RunProcess(GetFront(&que));
      }
      GetFront(&que)->running_time++;
    }
    Sleep(1);
  }
  DestructQueue(&que);
  return 0;
}

int ScheduleRR(Process *process, unsigned num_process) {
  static const unsigned kRoundRobin = 500;
  ProcessQueue que = ConstructQueue();
  unsigned ptr = 0;
  for (unsigned t = 0; ptr < num_process || que.size > 0; t++) {
    while (ptr < num_process && process[ptr].ready_time == t) {
      ForkProcess(&process[ptr]);
      EnQueue(&que, &process[ptr]);
      ptr++;
    }
    while (que.size > 0) {
      const Process *p = GetFront(&que);
      if (p->running_time < p->exec_time) break;
      waitpid(p->pid, NULL, 0);
      DeQueue(&que);
    }
    if (que.size > 0) {
      Process *p = GetFront(&que);
      if (p->running_time % kRoundRobin == 0 && p->status == RUNNING) {
        PauseProcess(p);
        DeQueue(&que);
        EnQueue(&que, p);
      }
    }
    if (que.size > 0) {
      Process *p = GetFront(&que);
      assert(p->running_time % kRoundRobin != 0 || p->status == WAITING);
      if (p->status == WAITING) {
        assert(RunProcess(p) == 0);
      }
      p->running_time++;
    }
    Sleep(1);
  }
  DestructQueue(&que);
  return 0;
}

int Schedule(Process *process, unsigned num_process, int policy) {
  if (policy == FIFO) return ScheduleFIFO(process, num_process);
  if (policy == RR) return ScheduleRR(process, num_process);
  // if (policy == SJF) return ScheduleSJF(process, num_process);
  // if (policy == PSJF) return SchedulePSJF(process, num_process);
  return -1;
}

#endif  // SCHEDULE_H_
