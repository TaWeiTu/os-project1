#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <sys/wait.h>

#include "process.h"
#include "queue.h"
#include "unit.h"

#define FIFO 0
#define RR 1
#define SJF 2
#define PSJF 3

static int ScheduleFIFO(Process *process, unsigned num_process) {
  ProcessQueue que = InitQueue();
  unsigned ptr = 0;
  for (unsigned t = 0; ptr < num_process || que.size > 0; t++) {
    while (ptr < num_process && process[ptr].ready_time == t) {
      ForkProcess(&process[ptr]);
      EnQueue(&que, &process[ptr]);
      ptr++;
    }
    while (que.size > 0) {
      const Process *p = GetFront(&que);
      if (p->running_time < p->exec_time) continue;
      waitpid(p->pid, NULL, 0);
    }
    if (que.size > 0) {
      GetFront(&que)->running_time++;
    }
    Sleep(1);
  }
}

int Schedule(Process *process, unsigned num_process, int policy) {
  if (policy == FIFO) return ScheduleFIFO(process, num_process);
  // if (policy == RR) return ScheduleRR(process, num_process);
  // if (policy == SJF) return ScheduleSJF(process, num_process);
  // if (policy == PSJF) return SchedulePSJF(process, num_process);
  return -1;
}

#endif  // SCHEDULE_H_
