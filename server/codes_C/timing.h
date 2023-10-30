#ifndef TIMING_H_
#define TIMING_H_

#include <time.h>
#include <stdint.h>


void static inline nwait(uint64_t nsec) {
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  uint64_t start_ns, current_ns;
  start_ns = ts.tv_nsec + ts.tv_sec * 1000000000;
  while (1) {
    timespec_get(&ts, TIME_UTC);
    current_ns = ts.tv_nsec + ts.tv_sec * 1000000000;
    if (current_ns - start_ns >= nsec) {
      break;
    }
  }
}

void static inline mwait(uint64_t msec) {
  nwait(msec * 1000000);
}

uint64_t static inline get_time_ns() {
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  return ts.tv_nsec + ts.tv_sec * 1000000000;
}

uint64_t static inline get_time_ms() {
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  return ts.tv_nsec / 1000000 + ts.tv_sec * 1000;
}

#endif // TIMING_H_