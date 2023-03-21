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

    // printf("%d\n", (int) (acos(-1.0) * 1000));
    // printf("%d\n", (int) (asin(0.0) * 1000));
    // printf("%d\n", (int) (acos(1.0) * 1000));
    // printf("%d\n", (int) (asin(-1.00) * 1000));
    // printf("%d\n", (int) (asin(0.0) * 1000));
    // printf("%d\n", (int) (asin(1.00) * 1000));
    // printf("%d\n", (int) (atan(-10.200) * 1000));
    // printf("%d\n", (int) (atan(-10.00) * 1000));
    // printf("%d\n", (int) (atan(0.0) * 1000));
    // printf("%d\n", (int) (atan(10.00) * 1000));
    // printf("%d\n", (int) (atan(10.200) * 1000));
    printf("%d\n", (int) (sqrt(2.14) * tan(270.2)) * 1000);

    print_title();
    print_start_calibration();

    controls_init();

    screen_init();
    loop_test();

    uart_putchar(EOT);
}
