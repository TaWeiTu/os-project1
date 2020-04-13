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
      fprintf(stderr, "[FIFO] EnQueue %s\n", process[ptr].name);
      ForkProcess(&process[ptr]);
      EnQueue(&que, &process[ptr]);
      ptr++;
    }
    while (que.size > 0) {
      const Process *p = GetFront(&que);
      if (p->remaining_time > 0) break;
      waitpid(p->pid, NULL, 0);
      DeQueue(&que);
      fprintf(stderr, "[FIFO] DeQueue %s\n", p->name);
    }
    if (que.size > 0) {
      if (GetFront(&que)->status == WAITING) {
        RunProcess(GetFront(&que));
      }
      GetFront(&que)->remaining_time--;
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
      fprintf(stderr, "[RR] EnQueue %s\n", process[ptr].name);
      ForkProcess(&process[ptr]);
      EnQueue(&que, &process[ptr]);
      ptr++;
    }
    while (que.size > 0) {
      const Process *p = GetFront(&que);
      if (p->remaining_time > 0) break;
      waitpid(p->pid, NULL, 0);
      fprintf(stderr, "[RR] DeQueue %s\n", p->name);
      DeQueue(&que);
    }
    if (que.size > 0) {
      Process *p = GetFront(&que);
      if ((p->exec_time - p->remaining_time) % kRoundRobin == 0 &&
          p->status == RUNNING) {
        fprintf(stderr, "[RR] Pause %s\n", p->name);
        PauseProcess(p);
        DeQueue(&que);
        EnQueue(&que, p);
      }
    }
    if (que.size > 0) {
      Process *p = GetFront(&que);
      assert((p->exec_time - p->remaining_time) % kRoundRobin != 0 ||
             p->status == WAITING);
      if (p->status == WAITING) {
        fprintf(stderr, "[RR] Run %s\n", p->name);
        assert(RunProcess(p) == 0);
      }
      p->remaining_time--;
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
  for (unsigned t = 0; ptr < num_process || heap.size > 0 || running; t++) {
    while (ptr < num_process && process[ptr].ready_time == t) {
      fprintf(stderr, "[SJF] Push %s\n", process[ptr].name);
      ForkProcess(&process[ptr]);
      HeapPush(&heap, &process[ptr]);
      ptr++;
    }
    if (running && running->remaining_time == 0) {
      waitpid(running->pid, NULL, 0);
      fprintf(stderr, "[SJF] wait %s\n", running->name);
      running = NULL;
    }
    if (!running && heap.size > 0) {
      Process *p = HeapGet(&heap);
      fprintf(stderr, "[SJF] Run %s\n", p->name);
      RunProcess(p);
      running = p;
      HeapPop(&heap);
    }
    if (running) running->remaining_time--;
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
    while (heap.size > 0) {
      Process *p = HeapGet(&heap);
      if (p->remaining_time > 0) break;
      assert(p == running);
      running = NULL;
      fprintf(stderr, "[PSJF] Wait %s\n", p->name);
      waitpid(p->pid, NULL, 0);
      HeapPop(&heap);
    }
    while (ptr < num_process && process[ptr].ready_time == t) {
      fprintf(stderr, "[PSJF] Fork %s\n", process[ptr].name);
      ForkProcess(&process[ptr]);
      HeapPush(&heap, &process[ptr]);
      ptr++;
    }
    if (heap.size > 0) {
      Process *p = HeapGet(&heap);
      if (p->status == WAITING) {
        if (running) {
          fprintf(stderr, "[PSJF] Pause %s\n", running->name);
          PauseProcess(running);
        }
        RunProcess(p);
      }
      running = p;
    } else {
      running = NULL;
    }
    if (running) running->remaining_time--;
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
