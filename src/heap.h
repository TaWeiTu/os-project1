#ifndef HEAP_H_
#define HEAP_H_

#include "vector.h"

typedef Vector ProcessHeap;

Vector ConstructHeap() { return ConstructVector(); }
void DestructHeap(Vector *vec) { DestructVector(vec); }

void HeapPush(Vector *vec, Process *process) {
  unsigned pos = vec->size;
  PushBack(vec, process);
  while (pos > 0 && GetProcess(vec, (pos - 1) / 2)->exec_time >
                        GetProcess(vec, pos)->exec_time) {
    SetProcess(vec, pos, GetProcess(vec, (pos - 1) / 2));
    pos = (pos - 1) / 2;
  }
  SetProcess(vec, pos, process);
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
    if (nxt + 1 < vec->size &&
        GetProcess(vec, nxt + 1)->exec_time < GetProcess(vec, nxt)->exec_time)
      nxt++;
    Process *child = GetProcess(vec, nxt);
    if (last->exec_time <= child->exec_time) break;
    SetProcess(vec, pos, child);
    pos = nxt;
  }
  if (vec->size > 0) SetProcess(vec, pos, last);
}

#endif  // HEAP_H_
