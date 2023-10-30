#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>
#include "dht11.h"
#include "28byj-48.h"

#define SUCCESS 0
#define INVALID_ARGS 1

stepper_handle_t stepper_handle;

void open_window() {
  stepper_run(512, &stepper_handle);
}

void close_window() {
  stepper_run(-512, &stepper_handle);
}

// first arg is the state of the winodw
int main(int argc, char const* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <state>\n", argv[0]);
    return INVALID_ARGS;
  }
  stepper_init(&stepper_handle, 1, 7, 8, 25, FULL_STEP, 2);
  int state = atoi(argv[1]);
  if (state) {
    open_window();
  } else {
    close_window();
  }
  return 0;
}
