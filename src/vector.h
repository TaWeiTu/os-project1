#ifndef VECTOR_H_
#define VECTOR_H_

#include "process.h"

static const unsigned kInitCap = 4;

typedef struct Vector {
  Process **process;
  unsigned size, cap;
} Vector;

Vector ConstructVector() {
  Vector vec;
  vec.process = (Process **)malloc(kInitCap * sizeof(Process *));
  for (unsigned i = 0; i < kInitCap; ++i) vec.process[i] = NULL;
  vec.size = 0;
  vec.cap = kInitCap;
  return vec;
}

Vector DestructVector(Vector *vec) { free(vec->process); }

void PushBack(Vector *vec, Process *p) {
  if (vec->size == vec->cap) {
    vec->cap <<= 1;
    vec->process =
        (Process **)realloc(vec->process, vec->cap * sizeof(Process *));
  }
  vec->process[vec->size++] = p;
}

void PopBack(Vector *vec) {
  assert(vec->size > 0);
  vec->size--;
}

Process *GetProcess(Vector *vec, unsigned idx) {
  assert(idx < vec->size);
  return vec->process[idx];
}

void SetProcess(Vector *vec, unsigned idx, Process *process) {
  assert(idx < vec->size);
  vec->process[idx] = process;
}

#endif  // VECTOR_H_
