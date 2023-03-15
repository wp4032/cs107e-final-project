#include "timer.h"
#include "uart.h"
#include "accel.h"
#include "mymodule.h"

void main(void)
{
    uart_init();
    timer_init();
    accel_init();
    uart_putchar(EOT);
}
