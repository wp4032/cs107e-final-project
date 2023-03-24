/* Simple SPI + MCP3008 ADC test. */

#include "gpio.h"
#include "uart.h"
#include "mcp3008.h"
#include "printf.h"

void main(void) 
{
    gpio_init();
    uart_init();
    mcp3008_init();

    while (1) {
        printf("Channel 0: %d\n", mcp3008_read(0));
    }
}
