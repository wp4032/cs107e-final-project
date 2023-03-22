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
#include "assert.h"
#include "gl.h"


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

void test_draw(void) {
    screen_init();
    screen_clear();

    printf("Drawing line 1");
    gl_draw_line(20, 30, 300, 500, GL_RED);
    printf("Drawing line 2");
    gl_draw_line(300, 500, 80, 600, GL_GREEN);

    printf("Drawing circle 1");
    gl_draw_circle_outline(500, 500, 10, GL_WHITE);
    printf("Drawing circle 2");
    gl_draw_circle_filled(500, 300, 100, GL_WHITE);
    printf("Drawing circle 3");
    gl_draw_circle_filled(20, 20, 0, GL_WHITE);
    printf("Drawing circle 4");
    gl_draw_circle_filled(40, 20, 2, GL_WHITE);
    gl_swap_buffer();
    timer_delay(10);
}

void main(void) 
{
    uart_init();
    // timer_init();
    // i2c_init();
    // gpio_init();
    // peripherals_init();
    // math_init();

    // test_floor();
    test_draw();

    uart_putchar(EOT);
}
