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
#include "title.h"

/*
 * Testing program for the 
 * DREAM'S DON'T DIE Moonwalker Brain Machine Interface
 *
 * Author: William Pan <williampan@cs.stanford.edu>
 * Date: Mar 28, 2023
 */

void test_math(void) {
    printf("%d\n", (int) (acos(-1.0) * 1000));
    printf("%d\n", (int) (asin(0.0) * 1000));
    printf("%d\n", (int) (acos(1.0) * 1000));
    printf("%d\n", (int) (asin(-1.00) * 1000));
    printf("%d\n", (int) (asin(0.0) * 1000));
    printf("%d\n", (int) (asin(1.00) * 1000));
    printf("%d\n", (int) (atan(-10.200) * 1000));
    printf("%d\n", (int) (atan(-10.00) * 1000));
    printf("%d\n", (int) (atan(0.0) * 1000));
    printf("%d\n", (int) (atan(10.00) * 1000));
    printf("%d\n", (int) (atan(10.200) * 1000));
    printf("%d\n", (int) (sqrt(2.14) * tan(270.2)) * 1000);

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

void test_messages(void) {
    // accel_loop_angles();

    screen_init();
    screen_clear();

    print_start_calibration();
}

void test_title(void) {
    screen_init();
    screen_clear();
    
    draw_title();
}

void main(void) 
{
    uart_init();
    timer_init();
    i2c_init();
    accel_init();
    gpio_init();
    peripherals_init();
    math_init();

    // test_floor();
    // test_draw();
    // test_messages();
    test_title();

    uart_putchar(EOT);
}
