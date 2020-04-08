#ifndef UNIT_H_
#define UNIT_H_

void Sleep(unsigned time) {
  for (unsigned t = 0; t < time; ++t) {
    for (volatile unsigned long i = 0; i < 1000000UL; ++i)
      ;
  }
}

#endif  // UNIT_H_
