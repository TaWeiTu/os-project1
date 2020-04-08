#ifndef QUEUE_H_
#define QUEUE_H_

#include <assert.h>

#include "process.h"

typedef struct QueueNode {
  struct QueueNode *nxt;
  Process *process;
} QueueNode;

typedef struct {
  QueueNode *head;
  QueueNode *rear;
  unsigned size;
} ProcessQueue;

ProcessQueue InitQueue() {
  ProcessQueue res;
  res.head = NULL;
  res.rear = NULL;
  res.size = 0;
  return res;
}

Process *GetFront(ProcessQueue *que) {
  assert(que->size > 0);
  return que->head->process;
}

void EnQueue(ProcessQueue *que, Process *process) {
  QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
  node->nxt = NULL;
  node->process = process;
  if (!que->rear) que->rear = node;
  if (!que->head) que->head = node;
  que->size++;
}

const Process *DeQueue(ProcessQueue *que) {
  assert(que->size > 0);
  const Process *process = que->head->process;
  que->head = que->head->nxt;
  que->size--;
  return process;
}

#endif  // QUEUE_H_
