#include "uart.h"
#include "malloc.h"
#include "gpio.h"

// linker memmap places these symbols at start/end of bss
extern char __bss_start__, __bss_end__;

extern void main(void);

// Addition added for mini-Valgrind memory report from assignment 4
// void memory_report (void);

// The C function _cstart is called from the assembly in start.s
// _cstart zeroes out the BSS section and then calls main.
// After return from main(), turns on the green ACT LED as
// a sign of successful completion.
void _cstart(void)
{
    char *bss = &__bss_start__;
    char *bss_end = &__bss_end__;

    while (bss < bss_end) {
        *bss++ = 0;
    }

    main();

    // memory_report();            // Addition added for mini-Valgrind memory report from assignment 4
    // uart_putchar(EOT);

    // Turn on the green ACT LED (GPIO 47), success LED (GPIO 12), and
    gpio_set_output(GPIO_PIN47);
    gpio_set_output(GPIO_PIN12);
    gpio_set_output(GPIO_PIN6);

    gpio_write(GPIO_PIN47, 1);
    gpio_write(GPIO_PIN12, 1);
    gpio_write(GPIO_PIN6, 1);
}
