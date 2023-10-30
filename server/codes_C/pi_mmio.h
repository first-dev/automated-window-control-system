// Simple fast memory-mapped GPIO library for the Raspberry Pi.
#ifndef PI_MMIO_H
#define PI_MMIO_H

#include <stdint.h>
#include <timing.h>

// Memory mapping success.
#define MMIO_SUCCESS 0
// Error opening /dev/gpiomem.
#define MMIO_ERROR_DEVMEM -1
// Memory mapping failed.
#define MMIO_ERROR_MMAP -2
// Timeout error.
#define TIMEOUT_ERROR -3

#define RISING_EDGE 1
#define FALLING_EDGE 2
#define BOTH_EDGES 3

// No timeout.
#define NO_TIMEOUT 0


extern volatile uint32_t* pi_mmio_gpio;

int pi_mmio_init(void);

static inline void pi_mmio_set_input(const int gpio_number) {
  // Set GPIO register to 000 for specified GPIO number.
  // uint32_t t = (pi_mmio_gpio + ((gpio_number) / 10));
  // uint32_t m = ~(7 << (((gpio_number) % 10) * 3));
  *(pi_mmio_gpio + ((gpio_number) / 10)) &= ~(7 << (((gpio_number) % 10) * 3));
}

static inline void pi_mmio_set_output(const int gpio_number) {
  // First set to 000 using input function.
  pi_mmio_set_input(gpio_number);
  // Next set bit 0 to 1 to set output.
  *(pi_mmio_gpio + ((gpio_number) / 10)) |= (1 << (((gpio_number) % 10) * 3));
}

static inline void pi_mmio_set_high(const int gpio_number) {
  *(pi_mmio_gpio + 7) = 1 << gpio_number;
}

static inline void pi_mmio_set_low(const int gpio_number) {
  *(pi_mmio_gpio + 10) = 1 << gpio_number;
}

static inline uint8_t pi_mmio_input(const int gpio_number) {
  return (*(pi_mmio_gpio + 13) & (1 << gpio_number)) ? 1 : 0;
}

uint8_t pi_mmio_wait_edge(const uint8_t gpio_number, const uint8_t edge_type, const uint64_t timeout_ns);

#endif
