#include "dht11.h"

int main(int argc, char const* argv[]) {
  dht11_data_t dht11_data;
  dht11_line_t* dht11_line = init_dht11(14);
  int err = read_dht11(dht11_line, &dht11_data);
  if (err == 0) {
    printf("%d.0 %d.0", dht11_data.humidity_int, dht11_data.temperature_int);
  } else {
    printf("error: %d\n", err);
  }
  return 0;
}
