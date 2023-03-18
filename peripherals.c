#include "gpio.h"
#include "messages.h"
#include "printf.h"
#include "timer.h"
#include "peripherals.h"


// FUNCTION: peripherals_init
// PARAMS: void
// Initializes all of the GPIOs and peripherals
void peripherals_init(void) {
  gpio_set_output(ERROR_LED);
  gpio_set_output(SUCCESS_LED);
}

void error_led(void) {
  gpio_write(ERROR_LED, 1);
  print_error();
  while (1) {
  }
}

void success_led_on(void) {
  gpio_write(SUCCESS_LED, 1);
}

void success_led_off(void) {
  gpio_write(SUCCESS_LED, 0);
}
