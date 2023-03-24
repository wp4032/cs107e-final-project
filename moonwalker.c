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
#include "interrupts.h"
#include "title.h"

void main(void) 
{
    interrupts_init();
    uart_init();
    timer_init();
    i2c_init();
    accel_init();
    gpio_init();
    peripherals_init();
    math_init();

    // Enables interrupts
    interrupts_global_enable();

    // Initializes the screen
    screen_init();

    // Draws the title screen
    draw_title();

    // Initializes the device and then loops to drraw
    controls_init();
    screen_loop();

    uart_putchar(EOT);
}
