#include "timer.h"
#include "uart.h"
#include "accel.h"
#include "i2c.h"
#include "printf.h"
#include "messages.h"
#include "controls.h"
#include "gpio.h"
#include "peripherals.h"
#include "screen.h"
#include "math.h"

void main(void) 
{
    uart_init();
    timer_init();
    i2c_init();
    accel_init();
    gpio_init();
    peripherals_init();
    math_init();
    screen_init();

    print_title();

    controls_init();
    // accel_loop_angles();

    uart_putchar(EOT);
}
