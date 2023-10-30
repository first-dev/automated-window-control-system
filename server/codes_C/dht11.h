#ifndef DHT11_H_
#define DHT11_H_

#include "timing.h"
#include "pi_mmio.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define SEC_TO_US(sec) ((sec)*1000000)
#define NS_TO_US(ns)    ((ns)/1000)

// the response signal containts 40 bits
#define RESPONSE_BIT_COUNT 40
// max pulse width before timing out
#define MAX_PULSE_WIDTH_CYCLES 10000
// max number of pulses in the response signal
#define MAX_PULSES_RESPONSE 84
// min number of pulses in the response signal otherwise it's too short
#define MIN_PULSES_RESPONSE 80
// default 1 wire GPIO line
#define DATA_LINE 14

#define SUCCESS 0
#define SHORT_RESPONSE_SIGNAL -1
#define INVALID_CHECKSUM -2

struct dht11_handle {
  int gpio_number;
};
typedef struct dht11_handle dht11_line_t;

struct dht11_data {
  uint8_t humidity_int;
  uint8_t humidity_dec;
  uint8_t temperature_int;
  uint8_t temperature_dec;
  uint8_t status_code;
};
typedef struct dht11_data dht11_data_t;


/**
  * @brief Initialize the dht11 line
  * @param line_num: the number of the gpio line
  * @return dht11 handle or NULL if an error occurred
*/
dht11_line_t* init_dht11(int line_num);

/**
  * @brief Read data from the dht11 module, if the reading fails, calling this function again could result in a successful reading
  * @param dht11_line: the dht11 handle
  * @return 0 if successful, negative value otherwise (check error codes)
*/
int read_dht11(dht11_line_t* dht11_line, dht11_data_t* dht11_data);

#endif // DHT11_H_