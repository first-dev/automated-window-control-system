#ifndef _28BYJ_48_H_
#define _28BYJ_48_H_

#include "pi_mmio.h"
#include "timing.h"
#include <stdint.h>


#define _28BYJ_48_STEPS_PER_REVOLUTION 2038

#define FULL_STEP 0
#define HALF_STEP 1
#define WAVE_DRIVE 2

#define MIN_FULL_STEP_TIME_MS 2
#define MIN_HALF_STEP_TIME_MS 1

struct stepper_handle {
  uint8_t in1;
  uint8_t in2;
  uint8_t in3;
  uint8_t in4;
  uint8_t mode;
  uint16_t step_time_ms;
};
typedef struct stepper_handle stepper_handle_t;

void stepper_init(stepper_handle_t* stepper_handle, uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4, uint8_t mode, uint16_t step_time_ms);
void stepper_run(int64_t steps, stepper_handle_t* stepper_handle);
void stepper_run_forever(stepper_handle_t* stepper_handle, uint8_t direction);


#endif // _28BYJ_48_H_