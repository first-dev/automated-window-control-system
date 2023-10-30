#include "dht11.h"

dht11_line_t dht11_handle;

uint8_t bits_to_byte(uint8_t* bits) {
  uint8_t byte = 0;
  for (int i = 0; i < 8; i++) {
    byte <<= 1;
    byte |= bits[i];
  }
  return byte;
}

int send_start_signal() {
  pi_mmio_set_output(dht11_handle.gpio_number);
  pi_mmio_set_low(dht11_handle.gpio_number);
  nwait(18000000);
  pi_mmio_set_high(dht11_handle.gpio_number);
  pi_mmio_set_input(dht11_handle.gpio_number);
  return 0;
}

int recieve_response_signal(dht11_data_t* dht11_data) {
  // we calculate pulse widths by incrementing a counter until the input value changes
  pi_mmio_set_input(dht11_handle.gpio_number);
  uint64_t init_value;
  uint64_t pulses_widths[MAX_PULSES_RESPONSE] = { 0 };
  int i;
  for (i = 0; i < MAX_PULSES_RESPONSE; i++) {
    init_value = pi_mmio_input(dht11_handle.gpio_number);
    while (pi_mmio_input(dht11_handle.gpio_number) == init_value) {
      pulses_widths[i]++;
      if (pulses_widths[i] > MAX_PULSE_WIDTH_CYCLES) {
        goto timeout;
      }
    }
  }
timeout:
  pulses_widths[i] = 0;
  // since it's not always guaranteed to receive the full response signal
  // we count how many pulses we received
  int pulses_count = 0;
  for (int i = 0; i < MAX_PULSES_RESPONSE; i++) {
    if (pulses_widths[i] == 0) {
      break;
    }
    pulses_count++;
  }
  if (pulses_count < MIN_PULSES_RESPONSE) {
    return SHORT_RESPONSE_SIGNAL;
  }
  // calculate the average low pulse width
  uint64_t low_pulse_width = 0;
  for (int i = pulses_count - 1; i > pulses_count - MIN_PULSES_RESPONSE; i -= 2) {
    low_pulse_width += pulses_widths[i];
  }
  low_pulse_width /= MIN_PULSES_RESPONSE / 2;

  // a pulse width of 70us means 1
  // a pulse width of 26~28us means 0
  // all low pulses widths are about 50us
  uint8_t bits[RESPONSE_BIT_COUNT] = { 0 };
  for (int i = pulses_count - MIN_PULSES_RESPONSE; i < pulses_count; i += 2) {
    int j = i - (pulses_count - MIN_PULSES_RESPONSE);
    // if pulse_width > low_pulse_width then it must be 70us which is a 1
    // otherwise it's 0
    if (pulses_widths[i] > low_pulse_width) {
      bits[j / 2] = 1;
    } else {
      bits[j / 2] = 0;
    }
  }
  // convert the bits to bytes
  uint8_t humidity_int = bits_to_byte(bits);
  uint8_t humidity_dec = bits_to_byte(bits + 8);
  uint8_t temperature_int = bits_to_byte(bits + 16);
  uint8_t temperature_dec = bits_to_byte(bits + 24);
  uint8_t checksum = bits_to_byte(bits + 32);
  // validate checksum
  if (checksum != (humidity_int + humidity_dec + temperature_int + temperature_dec)) {
    return INVALID_CHECKSUM;
  }
  dht11_data->humidity_int = humidity_int;
  dht11_data->humidity_dec = humidity_dec;
  dht11_data->temperature_int = temperature_int;
  dht11_data->temperature_dec = temperature_dec;
  return SUCCESS;
}

dht11_line_t* init_dht11(int line_num) {
  pi_mmio_init();
  dht11_handle.gpio_number = line_num;
  return &dht11_handle;
}

int read_dht11(dht11_line_t* dht11_line, dht11_data_t* dht11_data) {
  send_start_signal();
  int err = recieve_response_signal(dht11_data);
}