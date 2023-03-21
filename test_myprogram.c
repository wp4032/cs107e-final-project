// #include "timer.h"
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
#include "assert.h"

void test_floor(void) {
    assert(floor(3.0) == 3);
    assert(floor(3.3) == 3);
    assert(floor(3.6) == 3);

    assert(round(3.0) == 3);
    assert(round(3.3) == 3);
    assert(round(3.6) == 4);

    assert(decimals(3.0) == 0.0);
    assert(decimals(3.3) == 0.3);
    assert(decimals(3.6) == 0.6);

    assert(decimals_flipped(3.0) == 1.0);
    assert(decimals_flipped(3.3) == 0.7);
    assert(decimals_flipped(3.6) == 0.4);
}

void main(void) 
{
    uart_init();
    // timer_init();
    i2c_init();
    gpio_init();
    peripherals_init();
    math_init();

    test_floor();

    uart_putchar(EOT);
}
