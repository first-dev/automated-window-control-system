#include "28byj-48.h"


static void wait(stepper_handle_t* stepper_handle) {
  mwait(stepper_handle->step_time_ms);
}

static void step(stepper_handle_t* stepper_handle, uint8_t in1_state, uint8_t in2_state, uint8_t in3_state, uint8_t in4_state) {
  if (in1_state) pi_mmio_set_high(stepper_handle->in1);
  else pi_mmio_set_low(stepper_handle->in1);
  if (in2_state) pi_mmio_set_high(stepper_handle->in2);
  else pi_mmio_set_low(stepper_handle->in2);
  if (in3_state) pi_mmio_set_high(stepper_handle->in3);
  else pi_mmio_set_low(stepper_handle->in3);
  if (in4_state) pi_mmio_set_high(stepper_handle->in4);
  else pi_mmio_set_low(stepper_handle->in4);
  wait(stepper_handle);
}

void stepper_init(stepper_handle_t* stepper_handle, uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4, uint8_t mode, uint16_t step_time_ms) {
  stepper_handle->in1 = in1;
  stepper_handle->in2 = in2;
  stepper_handle->in3 = in3;
  stepper_handle->in4 = in4;
  stepper_handle->mode = mode;
  stepper_handle->step_time_ms = step_time_ms;
  pi_mmio_init();
  pi_mmio_set_output(stepper_handle->in1);
  pi_mmio_set_output(stepper_handle->in2);
  pi_mmio_set_output(stepper_handle->in3);
  pi_mmio_set_output(stepper_handle->in4);
}

void stepper_run(int64_t steps, stepper_handle_t* stepper_handle) {
  uint8_t direction = steps > 0;
  steps = steps > 0 ? steps : -steps;
  if (direction) {
    if (stepper_handle->mode == FULL_STEP) {
      for (int i = 0; i < steps / 4; i++) {
        step(stepper_handle, 1, 1, 0, 0);
        step(stepper_handle, 0, 1, 1, 0);
        step(stepper_handle, 0, 0, 1, 1);
        step(stepper_handle, 1, 0, 0, 1);
      }
    } else if (stepper_handle->mode == HALF_STEP) {
      for (int i = 0; i < steps / 8; i++) {
        step(stepper_handle, 1, 0, 0, 0);
        step(stepper_handle, 1, 1, 0, 0);
        step(stepper_handle, 0, 1, 0, 0);
        step(stepper_handle, 0, 1, 1, 0);
        step(stepper_handle, 0, 0, 1, 0);
        step(stepper_handle, 0, 0, 1, 1);
        step(stepper_handle, 0, 0, 0, 1);
        step(stepper_handle, 1, 0, 0, 1);
      }
    } else if (stepper_handle->mode == WAVE_DRIVE) {
      for (int i = 0; i < steps / 4; i++) {
        step(stepper_handle, 1, 0, 0, 0);
        step(stepper_handle, 0, 1, 0, 0);
        step(stepper_handle, 0, 0, 1, 0);
        step(stepper_handle, 0, 0, 0, 1);
      }
    }
  } else {
    if (stepper_handle->mode == FULL_STEP) {
      for (int i = 0; i < steps / 4; i++) {
        step(stepper_handle, 1, 0, 0, 1);
        step(stepper_handle, 0, 0, 1, 1);
        step(stepper_handle, 0, 1, 1, 0);
        step(stepper_handle, 1, 1, 0, 0);
      }
    } else if (stepper_handle->mode == HALF_STEP) {
      for (int i = 0; i < steps / 8; i++) {
        step(stepper_handle, 1, 0, 0, 1);
        step(stepper_handle, 0, 0, 0, 1);
        step(stepper_handle, 0, 0, 1, 1);
        step(stepper_handle, 0, 0, 1, 0);
        step(stepper_handle, 0, 1, 1, 0);
        step(stepper_handle, 0, 1, 0, 0);
        step(stepper_handle, 1, 1, 0, 0);
        step(stepper_handle, 1, 0, 0, 0);
      }
    } else if (stepper_handle->mode == WAVE_DRIVE) {
      for (int i = 0; i < steps / 4; i++) {
        step(stepper_handle, 0, 0, 0, 1);
        step(stepper_handle, 0, 0, 1, 0);
        step(stepper_handle, 0, 1, 0, 0);
        step(stepper_handle, 1, 0, 0, 0);
      }
    }
  }
}
void stepper_run_forever(stepper_handle_t* motor_handle, uint8_t direction) {
  while (1) {
    stepper_run(direction ? INT64_MAX : INT64_MIN, motor_handle);
  }
}