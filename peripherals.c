#include "gpio.h"
#include "messages.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"
#include "peripherals.h"


// FUNCTION: peripherals_init
// PARAMS: void
// Initializes all of the GPIOs and peripherals
void peripherals_init(void) {
  gpio_set_output(ERROR_LED);
  gpio_set_output(SUCCESS_LED);
}


// FUNCTION: error_led
// PARAMS: void
// Shines the error LED and will stay in an infinite loop until shut down
void error_led(void) {
  gpio_write(ERROR_LED, 1);
  draw_error();
  while(1) {
  }
}


// FUNCTION: success_led_on
// PARAMS: void
// Turns on the success LED
void success_led_on(void) {
  gpio_write(SUCCESS_LED, 1);
}


// FUNCTION: success_led_off
// PARAMS: void
// Turns off the success LED
void success_led_off(void) {
  gpio_write(SUCCESS_LED, 0);
}
