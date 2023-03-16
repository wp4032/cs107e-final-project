#include "timer.h"
#include "uart.h"
#include "accel.h"
#include "i2c.h"
#include "mymodule.h"
#include "printf.h"
#include "messages.h"
#include "controls.h"

void main(void)
{
    uart_init();
    timer_init();
    i2c_init();
    accel_init();

    print_title();
    print_start_calibration();

    controls_init();

    loop();

    uart_putchar(EOT);
}
