#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "pi_mmio.h"

#define BASE 0x20000000
#define GPIO_BASE (BASE + 0x200000)
#define GPIO_LENGTH 4096

volatile uint32_t* pi_mmio_gpio = NULL;

int pi_mmio_init(void) {
  if (pi_mmio_gpio == NULL) {
    int fd;
    if (access("/dev/gpiomem", F_OK) != -1) {
      fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
    } else {
      fd = open("/dev/mem", O_RDWR | O_SYNC);
    }
    if (fd == -1) {
      return MMIO_ERROR_DEVMEM;
    }
    pi_mmio_gpio = (uint32_t*)mmap(NULL, GPIO_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    close(fd);
    if (pi_mmio_gpio == MAP_FAILED) {
      pi_mmio_gpio = NULL;
      return MMIO_ERROR_MMAP;
    }
  }
  return MMIO_SUCCESS;
}

uint8_t pi_mmio_wait_edge(const uint8_t gpio_number, const uint8_t edge_type, const uint64_t timeout_ns) {
  pi_mmio_set_input(gpio_number);
  uint8_t init_value = pi_mmio_input(gpio_number);
  uint64_t start_time = get_time_ns();
  while (1) {
    if (get_time_ns() - start_time >= timeout_ns && timeout_ns != NO_TIMEOUT) {
      return TIMEOUT_ERROR;
    }
    if (edge_type == RISING_EDGE) {
      if (init_value == 0) {
        if (pi_mmio_input(gpio_number) == 1) {
          return RISING_EDGE;
        }
      } else {
        init_value = pi_mmio_input(gpio_number);
      }
    } else if (edge_type == FALLING_EDGE) {
      if (init_value == 1) {
        if (pi_mmio_input(gpio_number) == 0) {
          return FALLING_EDGE;
        }
      } else {
        init_value = pi_mmio_input(gpio_number);
      }
    } else if (edge_type == BOTH_EDGES) {
      if (init_value != pi_mmio_input(gpio_number)) {
        return init_value == 0 ? RISING_EDGE : FALLING_EDGE;
      }
    }
  }
}
