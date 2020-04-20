#ifndef HEAP_H_
#define HEAP_H_

#include "utils.h"
#include "vector.h"

typedef Vector ProcessHeap;

Vector ConstructHeap() { return ConstructVector(); }
void DestructHeap(Vector *vec) { DestructVector(vec); }

void HeapPush(Vector *vec, Process *process) {
  unsigned pos = vec->size;
  PushBack(vec, process);
  while (pos > 0 && GetProcess(vec, (pos - 1) / 2)->remaining_time >
                        GetProcess(vec, pos)->remaining_time) {
    SwapProcess(vec, (pos - 1) / 2, pos);
    pos = (pos - 1) / 2;
  }
  SetProcess(vec, pos, process);
  eprintf("[DEBUG - Push] ");
  for (unsigned i = 0; i < vec->size; ++i) {
    eprintf("%u ", GetProcess(vec, i)->remaining_time);
  }
  eprintf("\n");
}

Process *HeapGet(Vector *vec) {
  assert(vec->size > 0);
  return GetProcess(vec, 0);
}

void HeapPop(Vector *vec) {
  assert(vec->size > 0);
  Process *last = GetProcess(vec, vec->size - 1);
  PopBack(vec);
  unsigned pos = 0;
  while (pos * 2 + 1 < vec->size) {
    unsigned nxt = pos * 2 + 1;
    if (nxt + 1 < vec->size && GetProcess(vec, nxt + 1)->remaining_time <
                                   GetProcess(vec, nxt)->remaining_time)
      nxt++;
    Process *child = GetProcess(vec, nxt);
    if (last->remaining_time <= child->remaining_time) break;
    SetProcess(vec, pos, child);
    pos = nxt;
  }
  if (vec->size > 0) SetProcess(vec, pos, last);
  eprintf("[DEBUG - Pop] ");
  for (unsigned i = 0; i < vec->size; ++i) {
    eprintf("%u ", GetProcess(vec, i)->remaining_time);
  }
  eprintf("\n");
}

#endif  // HEAP_H_
