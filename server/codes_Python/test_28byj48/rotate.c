#include "../28byj-48.h"
#define IN1 1
#define IN2 7
#define IN3 8
#define IN4 25
#define STEP_TIME_MS 2

stepper_handle_t stepper_handle;

int main(int argc, char const* argv[]) {
  stepper_init(&stepper_handle, IN1, IN2, IN3, IN4, FULL_STEP, 2);
  stepper_run(1 * _28BYJ_48_STEPS_PER_REVOLUTION, &stepper_handle);
  stepper_run(-2 * _28BYJ_48_STEPS_PER_REVOLUTION, &stepper_handle);

  // while (1) {
  //   stepper_run(_28BYJ_48_STEPS_PER_REVOLUTION, &stepper_handle);
  //   mwait(1000);
  //   stepper_run(-_28BYJ_48_STEPS_PER_REVOLUTION, &stepper_handle);
  //   mwait(1000);
  // }
}
