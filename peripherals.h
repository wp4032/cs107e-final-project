#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "gpio.h"

/*
 * Controls the peripherals (GPIOs, etc) of the system
 * DREAM'S DON'T DIE Moonwalker Brain Machine Interface
 *
 * Author: William Pan <williampan@cs.stanford.edu>
 * Date: Mar 28, 2023
 */


#define ERROR_LED GPIO_PIN6
#define SUCCESS_LED GPIO_PIN12

void peripherals_init(void);

void error_led(void);
void success_led_on(void);
void success_led_off(void);

#endif
