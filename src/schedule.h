#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <assert.h>
#include <sys/wait.h>

#include "heap.h"
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
      if (GetFront(&que)->running_time == 0) {
        RunProcess(GetFront(&que));
      }
      GetFront(&que)->running_time++;
    }
    Sleep(1);
  }
  DestructQueue(&que);
  return 0;
}

static int ScheduleRR(Process *process, unsigned num_process) {
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

static int ScheduleSJF(Process *process, unsigned num_process) {
  ProcessHeap heap = ConstructHeap();
  unsigned ptr = 0;
  Process *running = NULL;
  for (unsigned t = 0; ptr < num_process || heap.size > 0; t++) {
    while (ptr < num_process && process[ptr].ready_time == t) {
      ForkProcess(&process[ptr]);
      HeapPush(&heap, &process[ptr]);
      ptr++;
    }
    if (running && running->running_time == running->exec_time) {
      waitpid(running->pid, NULL, 0);
      running = NULL;
    }
    if (!running && heap.size > 0) {
      Process *p = HeapGet(&heap);
      RunProcess(p);
      running = p;
      HeapPop(&heap);
    }
    if (running) running->running_time++;
    Sleep(1);
  }
  DestructHeap(&heap);
  return 0;
}

static int SchedulePSJF(Process *process, unsigned num_process) {
  ProcessHeap heap = ConstructHeap();
  unsigned ptr = 0;
  Process *running = NULL;
  for (unsigned t = 0; ptr < num_process || heap.size > 0; t++) {
    while (ptr < num_process && process[ptr].ready_time == t) {
      ForkProcess(&process[ptr]);
      HeapPush(&heap, &process[ptr]);
      ptr++;
    }
    while (heap.size > 0) {
      Process *p = HeapGet(&heap);
      if (p->running_time < p->exec_time) break;
      waitpid(p->pid, NULL, 0);
      HeapPop(&heap);
    }
    if (heap.size > 0) {
      Process *p = HeapGet(&heap);
      if (p->status == WAITING) {
        if (running) PauseProcess(running);
        RunProcess(p);
      }
      running = p;
    } else {
      running = NULL;
    }
    Sleep(1);
  }
  DestructHeap(&heap);
  return 0;
}

int Schedule(Process *process, unsigned num_process, int policy) {
  if (policy == FIFO) return ScheduleFIFO(process, num_process);
  if (policy == RR) return ScheduleRR(process, num_process);
  if (policy == SJF) return ScheduleSJF(process, num_process);
  if (policy == PSJF) return SchedulePSJF(process, num_process);
  return -1;
}

#endif  // SCHEDULE_H_
