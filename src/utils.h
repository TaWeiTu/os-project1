#ifndef UTILS_H_
#define UTILS_H_

#ifdef DEBUG
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#else
#define eprintf(...) 1
#endif

void SetAffinity(pid_t pid) {
  cpu_set_t mask;
  sched_getaffinity(pid, sizeof(cpu_set_t), &mask);
  cpu_set_t set_mask;
  CPU_ZERO(&set_mask);
  CPU_SET(0, &set_mask);
  sched_setaffinity(pid, sizeof(cpu_set_t), &set_mask);
  sched_getaffinity(pid, sizeof(cpu_set_t), &mask);
}


#endif  // UTILS_H_
